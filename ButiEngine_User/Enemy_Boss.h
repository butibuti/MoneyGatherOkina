#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class Enemy;

	class Enemy_Boss : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "Enemy_Boss";
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

		void Dead();
	private:
		void SetEnemyParameter();

		Value_weak_ptr<Enemy> m_vwp_enemyComponent;
	};
}

BUTI_REGIST_GAMECOMPONENT(Enemy_Boss, true);
