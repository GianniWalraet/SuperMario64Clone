#pragma once
#include "Prefabs/Character.h"

class ThirdPersonCamera;

class Mario final : public GameObject
{
public:
    Mario(const CharacterDesc& characterDesc, float cameraOffset = 50.f);
    virtual ~Mario() override = default;
    Mario(const Mario& other) = delete;
    Mario(Mario&& other) noexcept = delete;
    Mario& operator=(const Mario& other) = delete;
    Mario& operator=(Mario&& other) noexcept = delete;

    const int GetScore() const { return m_Score; }
    const uint8_t GetLives() const { return m_Lives; }

    void SetScore(int score) { m_Score = score; }
    void SetLives(uint8_t lives) { m_Lives = lives; }

    void IncrementScore() { ++m_Score; }
    void TakeDamage();
    void Jump();
protected:
    void Initialize(const SceneContext&) override;
    void Update(const SceneContext&) override;
private:
    CharacterDesc m_CharacterDesc;

    float m_TotalPitch{}, m_TotalYaw{};
    float m_MoveAcceleration{}, m_FallAcceleration{}, m_MoveSpeed{};
    XMFLOAT3 m_TotalVelocity{};
    XMFLOAT3 m_CurrentDirection{};

    float m_CamOffset{};
    int m_Score{};
    uint8_t m_Lives{3};

    ControllerComponent* m_pControllerComponent{};
    ThirdPersonCamera* m_pCamera{};
    CameraComponent* m_pCameraComponent{};

    GameObject* m_pCamPivot{};
    GameObject* m_pModel{};

    ParticleEmitterComponent* m_pJumpEmitter{};

    ModelAnimator* m_pAnimator{};
    int m_AnimationClipId{ 1 };
    std::unordered_map<std::wstring, int> m_AnimationIDs;

    std::wstring m_CurrentState{};
    std::wstring m_PreviousState{};

    uint8_t m_PreviousJumpCount{};
    uint8_t m_JumpCount{};
    uint8_t m_MaxJumps{ 2 };

    float m_GravityForce{ -3.f };

    FMOD::Sound* m_pJumpSound2D{};
    FMOD::Sound* m_pPainSound2D{};

    float m_IdleTime{};
    float m_MaxIdleTime{ 30.f };
    float m_MaxStunnedTime{ 2.f };
    float m_StunnedTime{};

    bool m_IsFalling{};
    bool m_IsStunned{};
    bool m_JumpTriggered{};

    void HandleInput(const SceneContext&);
    void HandleState();
};

