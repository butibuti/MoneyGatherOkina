#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {
class StageSelectManagerComponent;
class FadeOutComponent;
class SoundPlayerComponent;
class TitleManagerComponent : public GameComponent
{
public:
	std::string GetGameComponentName()const override {
		return "TitleManagerComponent";
	}
	void OnUpdate()override;
	void OnSet()override;
	void OnShowUI()override;
	void Start()override;
	bool IsTitle()const {
		return m_isTitle;
	}
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
	}

private:
	bool m_isTitle = true;
	Value_ptr<RelativeTimer> m_vlp_timer,m_vlp_selectUIApperTimer;
	Value_weak_ptr<StageSelectManagerComponent> vwp_stageselect;
	Value_weak_ptr<FadeOutComponent> m_vwp_fadeComponent;
	Value_weak_ptr<SoundPlayerComponent> m_vwp_soundPlayerComponent;
};
}

BUTI_REGIST_GAMECOMPONENT(TitleManagerComponent, true);
