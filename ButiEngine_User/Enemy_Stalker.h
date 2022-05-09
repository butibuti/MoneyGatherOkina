#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Enemy_Stalker :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Enemy_Stalker";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnRemove()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		void OnCollisionPlayer();
		void OnCollisionWorker();
		void OnCollisionEnemy();

		void SetEnemyParameter();

		bool m_isPrey;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy_Stalker, true);