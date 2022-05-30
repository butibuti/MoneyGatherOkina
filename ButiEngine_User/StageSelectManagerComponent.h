#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

class SceneChangeAnimationComponent;
class TitleManagerComponent;
class SoundPlayerComponent;

class StageSelectManagerComponent : public GameComponent
{
public:
	std::string GetGameComponentName()const override {
		return "StageSelectManagerComponent";
	}
	void OnUpdate()override;
	void OnSet()override;
	void OnShowUI()override;
	void Start()override;
	void End()override;
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
	}

	void NextScene();
	void BackScene();
	std::int16_t GetStageNum();

private:
	void FixStageNum();

	std::int16_t m_stageNum;
	std::int16_t m_maxStageNum;
	Value_ptr<Timer> m_waitTimer;
	Value_weak_ptr<SceneChangeAnimationComponent> m_vwp_gamePlayChangeAnimation;
	Value_weak_ptr<TitleManagerComponent> m_vwp_title;
	Value_weak_ptr<SoundPlayerComponent> m_vwp_soundPlayerComponent;
	//Value_weak_ptr<TitleManagerComponent> m_vwp_title;
	bool isSceneChange = false;
};
}

BUTI_REGIST_GAMECOMPONENT(StageSelectManagerComponent, true);

