#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
class SceneChangeAnimationComponent;
class LogoSceneManager :public GameComponent
{
public:
	LogoSceneManager() {}

	void OnUpdate()override;
	std::string GetGameComponentName()const override {
		return "LogoSceneManager";
	}
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
		archive(m_logoApperanceTime);
		archive(m_logoDisappearanceTime);
		archive(m_logokeepTime);
		archive(m_sceneChangeWait);
	}
	void OnShowUI()override;
	void Start()override;
	void OnSet()override;
private:
	Value_ptr<ButiRendering::CBuffer<ButiRendering::ObjectInformation>> m_vlp_logoBuffer, m_vlp_teamBuffer;
	Value_ptr<AbsoluteTimer> m_vlp_timer;
	Value_weak_ptr<SceneChangeAnimationComponent> m_vwp_sceneChangeAnimation;
	float m_logoApperanceTime,m_logokeepTime,m_logoDisappearanceTime,m_sceneChangeWait;
	std::vector<float> m_vec_timerCounts;
	std::vector<std::function<void()>>m_vec_timerUpdatingFunction,m_vec_timerResetFuncion;
	std::uint8_t m_index=0;
};

}
BUTI_REGIST_GAMECOMPONENT(LogoSceneManager,true)