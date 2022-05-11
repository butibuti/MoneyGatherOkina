#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Enemy;

	class Enemy_Volcano :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Enemy_Volcano";
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
		void ShotVolcanoRock();
		void AddPredictedPoint();
		void ScaleAnimation();

		void SetEnemyParameter();

		Value_ptr<Enemy> m_vlp_enemy;

		Vector3 m_randomVelocity;
		Vector3 m_defaultScale;
		Vector3 m_currentScale;
		Vector3 m_previousScale;

		std::int32_t m_rockShotCount;
		std::int32_t m_rockShotRate;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy_Volcano, true);

