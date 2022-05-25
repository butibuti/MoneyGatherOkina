#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;

	class Sensor :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Sensor";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		void OnCollisionFlocking(Value_weak_ptr<GameObject> arg_vwp_other);

		Value_weak_ptr<Player> m_vwp_player;
	};

}

BUTI_REGIST_GAMECOMPONENT(Sensor, true);