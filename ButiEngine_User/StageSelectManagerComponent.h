#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

class SceneChangeAnimationComponent;
class TitleManagerComponent;
class SoundPlayerComponent;
class EnemyScaleAnimationComponent;

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

	static std::int16_t m_stageNum;

	std::int16_t m_maxStageNum;
	Value_ptr<Timer> m_waitTimer;
	Value_weak_ptr<SceneChangeAnimationComponent> m_vwp_gamePlayChangeAnimation;
	Value_weak_ptr<TitleManagerComponent> m_vwp_title;
	Value_weak_ptr<SoundPlayerComponent> m_vwp_soundPlayerComponent;
	Value_weak_ptr<EnemyScaleAnimationComponent> m_vwp_enemyScaleAnimationComponent;
	Value_weak_ptr<EnemyScaleAnimationComponent> m_vwp_crystalScaleAnimationComponent;
	Value_weak_ptr<GameObject> m_vwp_kiba;
	Value_weak_ptr<GameObject> m_vwp_crystal;
	//Value_weak_ptr<TitleManagerComponent> m_vwp_title;

	Vector3 m_centerPosition;
	Vector3 m_rightPosition;
	Vector3 m_leftPosition;
	Vector3 m_kibaPosition;
	Vector3 m_crystalPosition;

	float m_scaleRate;
	float m_animationRate;
	bool m_isSceneChange = false;
};
}

BUTI_REGIST_GAMECOMPONENT(StageSelectManagerComponent, true);

