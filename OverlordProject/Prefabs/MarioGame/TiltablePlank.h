#pragma once

class TiltablePlank : public GameObject
{
public:
    TiltablePlank() = default;
    virtual ~TiltablePlank() override = default;
    TiltablePlank(const TiltablePlank& other) = delete;
    TiltablePlank(TiltablePlank&& other) noexcept = delete;
    TiltablePlank& operator=(const TiltablePlank& other) = delete;
    TiltablePlank& operator=(TiltablePlank&& other) noexcept = delete;
protected:
    void Initialize(const SceneContext&) override;
    void Update(const SceneContext&) override;
private:
    GameObject* m_pModel{};

    GameObject* m_Trigger1{};
    GameObject* m_Trigger2{};

    bool m_TiltLeft{};
    bool m_TiltRight{};
    bool m_TiltCenter{};
};

