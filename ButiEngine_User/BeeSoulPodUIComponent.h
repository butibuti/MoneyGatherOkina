#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;

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

		void AddSoulCount();

	private:
		void Animation();

		Value_weak_ptr<MeshDrawComponent> m_vwp_meshDrawComponent;
		Value_weak_ptr<Player> m_vwp_playerComponent;
		Value_weak_ptr<GameObject> m_vwp_player;

		float m_previousRate;
		float m_rate;

		std::int16_t m_soulCount;
		std::int16_t m_maxSoulCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(BeeSoulPodUIComponent, true);

