#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class AppealRBUI;

	class BeeSoulPodUIComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "BeeSoulPodUIComponent";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		float GetSoulRate() { return (float)m_soulCount / (float)m_maxSoulCount; }

		void AddSoulCount();
		void ResetSoulCount() { m_soulCount = 0; }

	private:
		void Animation();
		void ScaleAnimation();

		Value_weak_ptr<MeshDrawComponent> m_vwp_meshDrawComponent;
		Value_weak_ptr<AppealRBUI> m_vwp_appealRBUI;

		Vector3 m_currentScale;
		Vector3 m_defaultScale;

		float m_previousRate;
		float m_rate;

		std::int16_t m_soulCount;
		std::int16_t m_maxSoulCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(BeeSoulPodUIComponent, true);

