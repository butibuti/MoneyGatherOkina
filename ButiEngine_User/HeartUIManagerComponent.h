#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;

	class HeartUIManagerComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "HeartUIManagerComponent";
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

	private:
		void ResetHeartUI();

		Value_ptr<GameObject> m_vlp_player;
		Value_weak_ptr<Player> m_vwp_playerComponent;
		std::vector<Value_ptr<GameObject>> m_vec_vlp_heartUI;

		Vector3 m_heartUIPosition;
		Vector3 m_heartUIScale;

		float m_space;

		std::int8_t m_lifeCount;

	};

}

BUTI_REGIST_GAMECOMPONENT(HeartUIManagerComponent, true);

