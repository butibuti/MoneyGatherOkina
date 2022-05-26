#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Enemy;
	class ShakeComponent;

	class Enemy_Fly :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Enemy_Fly";
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

		void Dead();
	private:
		void SetEnemyParameter();
		void SetLoiterParameter();
		void SetLookAtParameter();

		static std::int32_t m_pocketCount;
		static float m_createPocketRadius;
		static float m_vibrationCapacity;
		static float m_vibrationResistance;

		Value_ptr<Enemy> m_vlp_enemy;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy_Fly, true);