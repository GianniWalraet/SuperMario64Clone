#pragma once


class Button2D;

class DefeatScreenHUD final : public GameObject
{
public:
    DefeatScreenHUD() = default;
    virtual ~DefeatScreenHUD() override = default;
    DefeatScreenHUD(const DefeatScreenHUD& other) = delete;
    DefeatScreenHUD(DefeatScreenHUD&& other) noexcept = delete;
    DefeatScreenHUD& operator=(const DefeatScreenHUD& other) = delete;
    DefeatScreenHUD& operator=(DefeatScreenHUD&& other) noexcept = delete;
protected:
    void Initialize(const SceneContext&) override;
    void Update(const SceneContext&) override;
private:
    Button2D* m_pQuitButton{};
    Button2D* m_pMainMenuButton{};
};


