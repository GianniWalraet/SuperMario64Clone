#include "stdafx.h"
#include "SpriteFontLoader.h"
#include <filesystem>

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	TODO_W5(L"Implement SpriteFontLoader >> Parse .fnt file")
	//See BMFont Documentation for Binary Layout
	//Parse the Identification bytes (B,M,F)
	auto B = pReader->Read<char>();
	auto M = pReader->Read<char>();
	auto F = pReader->Read<char>();
	//If Identification bytes doesn't match B|M|F,
	if (B != 'B' || M != 'M' || F != 'F')
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid.fnt font");
		return nullptr;
	}

	//Parse the version (version 3 required)
	auto version = pReader->Read<uint8_t>();
	//If version is < 3,
	if (version < 3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;
	}

	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	pReader->Read<uint8_t>();
	pReader->Read<int32_t>();
	//Retrieve the FontSize [fontDesc.fontSize]
	fontDesc.fontSize = pReader->Read<int16_t>();
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	pReader->MoveBufferPosition(12);
	//Retrieve the FontName [fontDesc.fontName]
	fontDesc.fontName = pReader->ReadNullString();

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	pReader->Read<uint8_t>();
	pReader->Read<int32_t>();
	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
	pReader->MoveBufferPosition(4);
	fontDesc.textureWidth = pReader->Read<uint16_t>();
	fontDesc.textureHeight = pReader->Read<uint16_t>();
	//Retrieve PageCount
	auto pageCount = pReader->Read<uint16_t>();
	if (pageCount > 1)
	{
		Logger::LogError(L"Only one texture per font is allowed!");
		return nullptr;
	}
	//Advance to Block2 (Move Reader)
	pReader->MoveBufferPosition(5);


	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	pReader->Read<uint8_t>();
	pReader->Read<int32_t>();
	//Retrieve the PageName (BinaryReader::ReadNullString)
	std::wstring pageName = pReader->ReadNullString();
	//  Construct the full path to the page texture file
	//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
	//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
	std::wstring fullPath = loadInfo.assetFullPath.parent_path().append(pageName);
	//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]
	fontDesc.pTexture = ContentManager::Load<TextureData>(fullPath);

	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	pReader->Read<uint8_t>();
	auto blockSize = pReader->Read<int32_t>();
	//Retrieve Character Count (see documentation)
	auto numChars = blockSize / 20;
	//Create loop for Character Count, and:
	for (size_t i = 0; i < numChars; i++)
	{
		//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		auto charId = static_cast<wchar_t>(pReader->Read<uint32_t>());
		//> Create instance of FontMetric (struct)
		FontMetric metric{};
		//	> Set Character (CharacterId) [FontMetric::character]
		metric.character = charId;
		//	> Retrieve Xposition (store Local)
		auto xPos = pReader->Read<uint16_t>();
		//	> Retrieve Yposition (store Local)
		auto yPos = pReader->Read<uint16_t>();
		//	> Retrieve & Set Width [FontMetric::width]
		metric.width = pReader->Read<uint16_t>();
		//	> Retrieve & Set Height [FontMetric::height]
		metric.height = pReader->Read<uint16_t>();
		//	> Retrieve & Set OffsetX [FontMetric::offsetX]
		metric.offsetX = pReader->Read<int16_t>();
		//	> Retrieve & Set OffsetY [FontMetric::offsetY]
		metric.offsetY = pReader->Read<int16_t>();
		//	> Retrieve & Set AdvanceX [FontMetric::advanceX]
		metric.advanceX = pReader->Read<int16_t>();
		//	> Retrieve & Set Page [FontMetric::page]
		metric.page = pReader->Read<uint8_t>();
		//	> Retrieve Channel (BITFIELD!!!) 
		auto channel = pReader->Read<uint8_t>();
		if (channel & 4)	  metric.channel = 0;
		else if (channel & 2) metric.channel = 1;
		else if (channel & 1) metric.channel = 2;
		else if (channel & 8) metric.channel = 3; 
		//	> See documentation for BitField meaning [FontMetrix::channel]
		//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
		XMFLOAT2 texCoord{};
		texCoord.x = (xPos == 0) ? 0.f : static_cast<float>(xPos) / fontDesc.textureWidth;
		texCoord.y = (yPos == 0) ? 0.f : static_cast<float>(yPos) / fontDesc.textureHeight;
		metric.texCoord = texCoord;
		//> Insert new FontMetric to the metrics [font.metrics] map
		//	> key = (wchar_t) charId
		//	> value = new FontMetric
		fontDesc.metrics[charId] = metric;
		//(loop restarts till all metrics are parsed)
	}

	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
