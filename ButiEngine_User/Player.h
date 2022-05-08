#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ParticleGenerater;

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
		float GetMoveSpeed() { return m_moveSpeed; }
		float GetVibrationForce() { return m_vibrationForce; }

		void AddExp();
		void KnockBack();
	private:
		void Move();
		void LevelUp();
		void MoveKnockBack();
		void TrajectoryParticleWaitCount();

		std::uint16_t CalcRequestExp();

		Value_weak_ptr<ParticleGenerater> m_vwp_particleGenerater;
		Vector3 m_knockBackVelocity;
		std::uint8_t m_life;
		std::uint8_t m_addTrajectoryParticleCounter;
		std::uint8_t m_addTrajectoryParticleWait;
		std::uint8_t m_knockBackFlame;
		std::uint8_t m_maxKnockBackFlame;

		std::uint8_t m_level;
		std::uint8_t m_maxLevel;
		std::uint8_t m_maxWorkerCount;
		std::uint16_t m_exp;

		float m_moveSpeed;
		float m_maxMoveSpeed;

		float m_vibrationForce;

		bool m_knockBackFlag;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player, true);