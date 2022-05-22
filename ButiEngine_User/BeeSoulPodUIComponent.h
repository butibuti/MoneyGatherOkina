#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

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

		void SetExpRate(const float arg_expRate);

	private:
		void Animation();

		Value_weak_ptr<MeshDrawComponent> m_vwp_meshDrawComponent;

		float m_previousRate;
		float m_rate;
	};

}

BUTI_REGIST_GAMECOMPONENT(BeeSoulPodUIComponent, true);

