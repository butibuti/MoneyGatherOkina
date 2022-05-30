#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class WaveManager;

	class OutsideCrystal :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "OutsideCrystal";
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

		void SetDefaultScale(const Vector3& arg_scale) { m_defaultScale = arg_scale; }

		bool IsInvincible() { return m_isInvincible; }

		void Appeaer();
		void Disappear();
		void SpawnStalker();
	private:
		void Wait();
		void SetEnemyParameter();

		static std::int32_t m_progressPoint;
		static std::int32_t m_pocketCount;
		static float m_createPocketRadius;
		static float m_vibrationCapacity;
		static float m_vibrationResistance;
		static std::int32_t m_appearIntervalFrame;
		static std::int32_t m_spwanStalkerCount;
		static float m_stalerKnockBackForce;
		static float m_knockBackY;

		Value_weak_ptr<WaveManager> m_vwp_waveManager;

		Value_ptr<Timer> m_vlp_appearIntervalTimer;
		Vector3 m_defaultScale;
		bool m_isAppear;
		bool m_isInvincible;
	};

}

BUTI_REGIST_GAMECOMPONENT(OutsideCrystal, true);