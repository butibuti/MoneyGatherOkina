#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Enemy;
	class Player;

	class PlayerSensor :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "PlayerSensor";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnRemove()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void SetParentEnemy(Value_weak_ptr<GameObject> arg_vwp_enemy);
	private:
		Value_weak_ptr<GameObject> m_vwp_enemy;
		Value_weak_ptr<GameObject> m_vwp_player;
		Value_ptr<Enemy> m_vlp_enemy;
		Value_ptr<Player> m_vlp_player;
	};

}

BUTI_REGIST_GAMECOMPONENT(PlayerSensor, true);