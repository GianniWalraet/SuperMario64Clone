#pragma once
#include "Prefabs/Character.h"

class Goomba final : public GameObject
{
public:
    Goomba(const CharacterDesc& characterDesc, float detectionRadius);
    virtual ~Goomba() override = default;

    Goomba(const Goomba& other) = delete;
    Goomba(Goomba&& other) noexcept = delete;
    Goomba& operator=(const Goomba& other) = delete;
    Goomba& operator=(Goomba&& other) noexcept = delete;

    void SetTarget(GameObject* pTarget) { m_pTarget = pTarget; }
    const bool IsDead() const { return m_IsDead; }

    void Kill() { m_MarkedForDelete = true; }
protected:
    void Initialize(const SceneContext&) override;
    void PostInitialize(const SceneContext&) override;
    void Update(const SceneContext&) override;
private:
    GameObject* m_pModel{};
    ControllerComponent* m_pControllerComponent{};

    float m_MoveAcceleration{}, m_FallAcceleration{};
    XMFLOAT3 m_TotalVelocity{};

    float m_MaxTiltAngle{ 10.f };
    float m_WiggleSpeed{ 4.f };

    FMOD::Sound* m_pStompSound2D{};

    CharacterDesc m_CharacterDesc;
    float m_MovementSpeed{};
    float m_ChaseSpeed{ 2.f };
    float m_WanderSpeed{ 1.f };
    float m_DetectionRadius{};



    // Kill detection
    GameObject* m_pKillTrigger{};
    PxShape* m_HeadCollider{};

    float ElapsedTime{};
    float m_DeathAnimationTime{ 1.f };

    bool m_MarkedForDelete{ false };
    bool m_IsDead{ false };

    // Attack detection
    GameObject* m_pAttackTrigger{};
    PxShape* m_BodyCollider{};

    bool m_IgnoreTarget{ false };
    float m_IgnoreTime{ 2.f };
    float m_ElapsedIgnoreTime{};

    // SteeringBehavior Variables
    float m_DetectionAngle{ 60.f };

    // Seek
    GameObject* m_pTarget{};
    XMFLOAT3 m_OriginalPos{};
    XMFLOAT3 m_TargetPos{};

    // Wander
    float m_WanderRadius{ 10.f };
    float m_WanderElapsedTime{};
    float m_WanderWaitTime{ };

    void OnDamageGiven(GameObject* pOther);
    void OnDie();

    void Wander(float dt);

    void UpdateCollisionBoxes();

    bool IsInView(const XMFLOAT3& pos);
    void HandleRotation(float totalTime);
    void HandleMovement(float dt);

    void MoveToTarget();
    void CalculateWanderTarget();
};

