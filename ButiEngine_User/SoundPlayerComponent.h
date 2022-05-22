#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

class SoundPlayerComponent : public GameComponent
{
public:
	std::string GetGameComponentName()const override {
		return "SoundPlayerComponent";
	}
	void OnSet()override;
	void OnShowUI()override;
	void Start()override;
	void PlayBGM(SoundTag arg_sound, const float arg_volume = 1.0f);
	void PlaySE(SoundTag arg_sound, const float arg_volume = 1.0f);
	float GetVolume(SoundTag arg_sound)const;
	void SetVolume(SoundTag arg_sound,const float arg_volume);
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
	}
private:
	std::unordered_map<std::string, float> m_umap_soundVolume;
};
}

BUTI_REGIST_GAMECOMPONENT(SoundPlayerComponent, true);