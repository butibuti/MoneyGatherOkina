#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Player";
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
		void Move();

		std::int8_t m_life;
		std::int8_t m_level;
		float m_moveSpeed;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player, true);