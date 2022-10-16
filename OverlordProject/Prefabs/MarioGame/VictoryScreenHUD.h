#pragma once

class Button2D;

class VictoryScreenHUD final : public GameObject
{
public:
    VictoryScreenHUD() = default;
    virtual ~VictoryScreenHUD() override = default;

    VictoryScreenHUD(const VictoryScreenHUD& other) = delete;
    VictoryScreenHUD(VictoryScreenHUD&& other) noexcept = delete;
    VictoryScreenHUD& operator=(const VictoryScreenHUD& other) = delete;
    VictoryScreenHUD& operator=(VictoryScreenHUD&& other) noexcept = delete;
protected:
    void Initialize(const SceneContext&) override;
    void Update(const SceneContext&) override;
private:
    Button2D* m_pQuitButton{};
    Button2D* m_pMainMenuButton{};
};

