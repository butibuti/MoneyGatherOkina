#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;

	class Bomb_Player :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Bomb_Player";
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

		void Appear();
		void Disappear();
	private:
		void AddScaleAnimation(const Vector3& arg_targetScale);

		Value_ptr<Player> m_vlp_player;
		Vector3 m_maxScale;
		bool m_isAppear;
	};

}

BUTI_REGIST_GAMECOMPONENT(Bomb_Player, true);