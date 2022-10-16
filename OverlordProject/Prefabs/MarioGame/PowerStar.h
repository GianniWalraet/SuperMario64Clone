#pragma once
class PowerStar final : public GameObject
{
public:
    PowerStar() = default;
    virtual ~PowerStar() override = default;

    PowerStar(const PowerStar& other) = delete;
    PowerStar(PowerStar&& other) noexcept = delete;
    PowerStar& operator=(const PowerStar& other) = delete;
    PowerStar& operator=(PowerStar&& other) noexcept = delete;

    void MarkForDelete() { m_MarkForDelete = true; }
protected:
    void Initialize(const SceneContext&) override;
    void Update(const SceneContext&) override;
private:
    float m_FlySpeed{ 5.f };
    float m_RotationSpeed{ 5.f };
    bool m_MarkForDelete{};

    ParticleEmitterComponent* m_pStarsEmitter{};

    FMOD::Channel* m_pPickUpSound2D{};
    FMOD::Channel* m_pMarioHaha2D{};

    void PlayPickupAnimation(float elapsedTime);
};

