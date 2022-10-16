#pragma once

class PostScanLine : public PostProcessingMaterial
{
public:
	PostScanLine();
	~PostScanLine() override = default;
	PostScanLine(const PostScanLine& other) = delete;
	PostScanLine(PostScanLine&& other) noexcept = delete;
	PostScanLine& operator=(const PostScanLine& other) = delete;
	PostScanLine& operator=(PostScanLine&& other) noexcept = delete;

protected:
	void Initialize(const GameContext& /*gameContext*/) override {}
};

