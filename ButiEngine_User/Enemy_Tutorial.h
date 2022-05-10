#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Enemy;

	class Enemy_Tutorial :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Enemy_Tutorial";
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
		void SetEnemyParameter();
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy_Tutorial, true);