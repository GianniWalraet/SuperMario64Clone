#pragma once

enum class PlayState
{
	Won,
	Lost,
	Playing
};

class GameState final
{
public:
	static GameState& Get()
	{
		static GameState instance{};
		return instance;
	}

	void SetPaused(bool isPaused) { m_IsPaused = isPaused; }
	const bool GetPaused() const { return m_IsPaused; }

	const PlayState GetGameState() const { return m_State; }
	void GetGameState(PlayState newState) { m_State = newState; }
private:
	GameState() = default;
	virtual ~GameState() = default;
	GameState(const GameState& other) = delete;
	GameState(GameState&& other) noexcept = delete;
	GameState& operator=(const GameState& other) = delete;
	GameState& operator=(GameState&& other) noexcept = delete;

	bool m_IsPaused{};
	PlayState m_State{ PlayState::Playing };
};
