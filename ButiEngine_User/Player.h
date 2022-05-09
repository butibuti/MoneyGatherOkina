#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ParticleGenerater;
	class WaveManager;

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

		std::uint8_t GetMaxWorkerCount() { return m_maxWorkerCount; }
		float GetMoveSpeed() { return m_velocity.GetLength(); }
		float GetMaxMoveSpeed() { return m_maxMoveSpeed; }
		float GetVibrationForce() { return m_vibrationForce; }

		void AddExp();
		void KnockBack();
	private:
		void Move();
		void LevelUp();
		void MoveKnockBack();
		void TrajectoryParticleWaitCount();

		std::uint16_t CalcRequestExp();

		Value_weak_ptr<WaveManager> m_vwp_waveManager;

		std::int8_t m_life;

		//レベル
		std::uint8_t m_level;
		std::uint8_t m_maxLevel;
		std::uint8_t m_maxWorkerCount;
		std::uint16_t m_exp;

		//移動速度
		Vector3 m_velocity;
		float m_maxMoveSpeed;
		float m_acceleration;
		float m_deceleration;


		//ノックバック
		Vector3 m_knockBackVelocity;
		std::int8_t m_knockBackFrame;
		std::int8_t m_maxKnockBackFrame;
		bool m_isKnockBack;

		//振動
		float m_vibrationForce;

		//パーティクル
		Value_weak_ptr<ParticleGenerater> m_vwp_particleGenerater;
		std::int8_t m_addTrajectoryParticleCounter;
		std::int8_t m_addTrajectoryParticleWait;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player, true);