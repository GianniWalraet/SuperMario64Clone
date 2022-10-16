#pragma once

class BobOmbBall : public GameObject
{
public:
    BobOmbBall(const std::deque<XMFLOAT3>& moveTargets, float speed);
    virtual ~BobOmbBall() override = default;
    BobOmbBall(const BobOmbBall& other) = delete;
    BobOmbBall(BobOmbBall&& other) noexcept = delete;
    BobOmbBall& operator=(const BobOmbBall& other) = delete;
    BobOmbBall& operator=(BobOmbBall&& other) noexcept = delete;
protected:
    void Initialize(const SceneContext&) override;
    void Update(const SceneContext&) override;
private:
    GameObject* m_pModel{};
    bool m_MarkedForDelete{};

    std::deque<XMFLOAT3> m_MoveTargets{};
    XMFLOAT3 m_CurrentTarget{};

    XMFLOAT3 m_MovementSpeed{ 0.f,0.f,0.f };
    XMFLOAT3 m_MoveDir{ 1.f,1.f,1.f };

    float m_YOffset{ 0.5f };

    float m_PrevDistance{ FLT_MAX };

    void SetNextTarget();
    void CheckNextPointReached();
};

