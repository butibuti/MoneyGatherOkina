#pragma once

class GameSettings
{
public:
	~GameSettings() {}

	static const ButiEngine::Color PLAYER_COLOR;
	static const ButiEngine::Color ATTACK_COLOR;
	static const ButiEngine::Color SOUL_COLOR;
private:
	GameSettings();
};