#pragma once

class GameSettings
{
public:
	~GameSettings() {}

	static const ButiEngine::Color PLAYER_COLOR;
	static const ButiEngine::Color PLAYER_ATTACK_COLOR;
	static const ButiEngine::Color ENEMY_ATTACK_COLOR;
	static const ButiEngine::Color WORKER_COLOR;
private:
	GameSettings();
};