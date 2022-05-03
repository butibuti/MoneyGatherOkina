#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RigidBodyComponent;

	class Player :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Player";
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

		float GetMoveSpeed() { return m_moveSpeed; }
	private:
		void Move();

		Value_ptr<RigidBodyComponent> m_vlp_rigidBody;
		std::int8_t m_life;
		std::int8_t m_level;
		float m_moveSpeed;
		float m_maxMoveSpeed;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player, true);