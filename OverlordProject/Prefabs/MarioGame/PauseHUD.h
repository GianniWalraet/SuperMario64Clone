#pragma once

class Button2D;

class PauseHUD : public GameObject
{
public:
    PauseHUD() = default;
    virtual ~PauseHUD() override = default;

    PauseHUD(const PauseHUD& other) = delete;
    PauseHUD(PauseHUD&& other) noexcept = delete;
    PauseHUD& operator=(const PauseHUD& other) = delete;
    PauseHUD& operator=(PauseHUD&& other) noexcept = delete;
protected:
    void Initialize(const SceneContext&) override;
    void Update(const SceneContext&) override;
    void Draw(const SceneContext&) override;
private:
    Button2D* m_pResumeButton{};
    Button2D* m_pMainMenuButton{};

    TextureData* m_pControlSchemeTex{};
    SpriteFont* m_pFont{};
};


