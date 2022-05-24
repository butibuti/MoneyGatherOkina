#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class Enemy;

	enum class BossState
	{
		Wait,
		Flirting,
		StrenghenWait,
		Fire,
		Crystal,
		Jump,
	};

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

		bool IsStrengthened() { return m_isStrengthened; }
		bool IsInAir();

		void Dead();
	private:
		void Wait();
		void WaitStart();
		void Flirting();
		void FlirtingStart();
		void StrengthenWait();
		void StrengthenWaitStart();

		void SetState();
		void DecreaseStatePercent(float& arg_percent, float& arg_otherPercent1, float& arg_otherPercent2);
		void RemoveAttackComponent();

		void SetEnemyParameter();
		void SetStateParameter();
		void CheckStrengthen();

		Value_weak_ptr<Enemy> m_vwp_enemyComponent;

		static std::int32_t m_pocketCount;
		static float m_createPocketRadius;

		//行動パターン
		Value_ptr<Timer> m_vlp_waitTimer;
		Value_ptr<Timer> m_vlp_flirtingTimer;
		Value_ptr<Timer> m_vlp_strengthenWaitTimer;
		BossState m_state;
		//パターンが出る確率
		float m_firePercent;
		float m_crystalPercent;
		float m_jumpPercent;

		std::int16_t m_life;
		std::int16_t m_maxLife;
		std::int16_t m_strengthenBorder;

		bool m_isStrengthened;

		bool m_isDead;
	};
}

BUTI_REGIST_GAMECOMPONENT(Enemy_Boss, true);
