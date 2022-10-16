#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount):
	m_ParticlesArray(new Particle[particleCount]),
	m_ParticleCount(particleCount), //How big is our particle buffer?
	m_MaxParticles(particleCount), //How many particles to draw (max == particleCount)
	m_AssetFile(assetFile),
	m_EmitterSettings(emitterSettings)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	//TODO_W9(L"Implement Destructor")
	delete[] m_ParticlesArray;
	m_pVertexBuffer->Release();
}

void ParticleEmitterComponent::Burst(float burstTime)
{
	m_IsBurstParticle = true;
	m_Enabled = true;
	m_Bursting = true;
	m_BurstTime = burstTime;
}

void ParticleEmitterComponent::Initialize(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement Initialize")
	if (!m_pParticleMaterial) m_pParticleMaterial = MaterialManager::Get()->CreateMaterial<ParticleMaterial>();
	CreateVertexBuffer(sceneContext);
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement CreateVertexBuffer")
	if (m_pVertexBuffer) m_pVertexBuffer->Release();

	D3D11_BUFFER_DESC desc{};
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = m_ParticleCount * sizeof(VertexParticle);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0x10000L;
	desc.MiscFlags = 0;

	HANDLE_ERROR(sceneContext.d3dContext.pDevice->CreateBuffer(&desc, nullptr, &m_pVertexBuffer));
}

void ParticleEmitterComponent::Update(const SceneContext& sceneContext)
{
	if (m_Bursting)
	{
		m_ElapsedTime += sceneContext.pGameTime->GetElapsed();
		if (m_ElapsedTime > m_BurstTime)
		{
			m_Enabled = false;
			m_Bursting = false;
			m_ElapsedTime = 0.f;
		}
	}

	//TODO_W9(L"Implement Update")
	float particleInterval = ((m_EmitterSettings.minEnergy + m_EmitterSettings.maxEnergy) / 2.f) / m_ParticleCount;

	m_LastParticleSpawn += sceneContext.pGameTime->GetElapsed();
	m_ActiveParticles = 0;

	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	sceneContext.d3dContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	VertexParticle* pBuffer = reinterpret_cast<VertexParticle*>(mappedResource.pData);

	const auto& elapsedTime = sceneContext.pGameTime->GetElapsed();
	for (size_t i = 0; i < m_ParticleCount; i++)
	{
		if (m_ParticlesArray[i].isActive)
			UpdateParticle(m_ParticlesArray[i], elapsedTime);

		if (!m_ParticlesArray[i].isActive && m_LastParticleSpawn >= particleInterval)
			SpawnParticle(m_ParticlesArray[i]);
		
		if (m_ParticlesArray[i].isActive)
			pBuffer[m_ActiveParticles++] = m_ParticlesArray[i].vertexInfo;
	}

	sceneContext.d3dContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::UpdateParticle(Particle& p, float elapsedTime) const
{
	//TODO_W9(L"Implement UpdateParticle")
	if (!p.isActive) return;

	p.currentEnergy -= elapsedTime;
	if (p.currentEnergy < 0)
	{
		p.isActive = false;
		return;
	}

	// Pos
	const auto& vel = m_EmitterSettings.velocity;
	p.vertexInfo.Position = { p.vertexInfo.Position.x + vel.x * elapsedTime, p.vertexInfo.Position.y + vel.y * elapsedTime, p.vertexInfo.Position.z + vel.z * elapsedTime };

	// Color
	float lifePercent = p.currentEnergy / p.totalEnergy;
	p.vertexInfo.Color = m_EmitterSettings.color;
	p.vertexInfo.Color.w = m_EmitterSettings.color.w * lifePercent;

	// Size
	p.vertexInfo.Size = std::lerp(p.initialSize, p.initialSize * p.sizeChange, 1.f - lifePercent);
}

void ParticleEmitterComponent::SpawnParticle(Particle& p)
{
	if (!m_Enabled && m_IsBurstParticle) return;

	//TODO_W9(L"Implement SpawnParticle")
	p.isActive = true;

	// Energy
	float randEnergy = MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);
	p.currentEnergy = randEnergy;
	p.totalEnergy = randEnergy;

	// Position
	XMVECTOR randDir = { 1,0,0 };
	auto randRotMat = XMMatrixRotationRollPitchYaw(MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI));
	XMVECTOR dir = XMVector3TransformNormal(randDir, randRotMat);
	
	auto pos = XMLoadFloat3(&GetTransform()->GetWorldPosition());

	auto distance = MathHelper::randF(m_EmitterSettings.minEmitterRadius, m_EmitterSettings.maxEmitterRadius);
	XMStoreFloat3(&p.vertexInfo.Position, pos + dir * distance );

	// Size
	float randSize = MathHelper::randF(m_EmitterSettings.minSize, m_EmitterSettings.maxSize);
	p.initialSize = randSize;
	p.vertexInfo.Size = randSize;

	p.sizeChange = MathHelper::randF(m_EmitterSettings.minScale, m_EmitterSettings.maxScale);

	// Rotation
	p.vertexInfo.Rotation = MathHelper::randF(-XM_PI, XM_PI);

	// Color
	p.vertexInfo.Color = m_EmitterSettings.color;
}

void ParticleEmitterComponent::PostDraw(const SceneContext& sceneContext)
{
	if (!m_Enabled) return;
	//TODO_W9(L"Implement PostDraw")
	m_pParticleMaterial->SetVariable_Matrix(L"gWorldViewProj", sceneContext.pCamera->GetViewProjection());
	m_pParticleMaterial->SetVariable_Matrix(L"gViewInverse", sceneContext.pCamera->GetViewInverse());
	m_pParticleMaterial->SetVariable_Texture(L"gParticleTexture", m_pParticleTexture);

	const auto& pDeviceContext = sceneContext.d3dContext.pDeviceContext;
	const auto& context = m_pParticleMaterial->GetTechniqueContext();

	//Set Inputlayout
	pDeviceContext->IASetInputLayout(context.pInputLayout);

	//Set Primitive Topology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//Set Vertex Buffer
	const UINT offset = 0;
	const UINT stride = sizeof(VertexParticle);
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//DRAW
	auto tech = m_pParticleMaterial->GetTechniqueContext().pTechnique;
	D3DX11_TECHNIQUE_DESC techDesc{};

	tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		tech->GetPassByIndex(p)->Apply(0, pDeviceContext);
		pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}

void ParticleEmitterComponent::DrawImGui()
{
	if(ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::SliderUInt("Count", &m_ParticleCount, 0, m_MaxParticles);
		ImGui::InputFloatRange("Energy Bounds", &m_EmitterSettings.minEnergy, &m_EmitterSettings.maxEnergy);
		ImGui::InputFloatRange("Size Bounds", &m_EmitterSettings.minSize, &m_EmitterSettings.maxSize);
		ImGui::InputFloatRange("Scale Bounds", &m_EmitterSettings.minScale, &m_EmitterSettings.maxScale);
		ImGui::InputFloatRange("Radius Bounds", &m_EmitterSettings.minEmitterRadius, &m_EmitterSettings.maxEmitterRadius);
		ImGui::InputFloat3("Velocity", &m_EmitterSettings.velocity.x);
		ImGui::ColorEdit4("Color", &m_EmitterSettings.color.x, ImGuiColorEditFlags_NoInputs);
	}
}