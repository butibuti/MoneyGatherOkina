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
	void OnRemove()override;
	void OnShowUI()override;
	void Start()override;
	void PlayBGM(SoundTag arg_sound, const float arg_volume = 1.0f);
	void PlaySE(SoundTag arg_sound, const float arg_volume = 1.0f);
	void PlayControllableSE(SoundTag arg_sound, const std::int32_t arg_index,  const float arg_volume = 1.0f, const bool arg_isLoop=false);
	void SetControllableSEVolume(const std::uint32_t arg_index, const float arg_volume) ;
	float GetVolume(SoundTag arg_sound)const;
	void SetVolume(SoundTag arg_sound,const float arg_volume);
	void SetLoopIndex(const std::string& arg_indexName);
	std::int32_t GetLoopIndex(const std::string& arg_indexName);
	void DestroyLoopIndex(const std::string& arg_indexName);
	void StopLoopSound();

	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(isActive);
	}
private:
	std::unordered_map<std::string, float> m_umap_soundVolume;
	std::vector<std::string> m_vec_loopIndices;
};
}

BUTI_REGIST_GAMECOMPONENT(SoundPlayerComponent, true);