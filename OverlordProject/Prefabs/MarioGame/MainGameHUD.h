#pragma once

class Mario;

class MainGameHUD : public GameObject
{
public:
    MainGameHUD(Mario* attachedPlayer);
    virtual ~MainGameHUD() override = default;

    MainGameHUD(const MainGameHUD& other) = delete;
    MainGameHUD(MainGameHUD&& other) noexcept = delete;
    MainGameHUD& operator=(const MainGameHUD& other) = delete;
    MainGameHUD& operator=(MainGameHUD&& other) noexcept = delete;
protected:
    void Initialize(const SceneContext&) override;
    void Update(const SceneContext&) override;
    void PostDraw(const SceneContext&) override;
private:
    Mario* m_pMario{};
    SpriteFont* m_pFont{};

    TextureData* m_MarioHead2D{};
};

