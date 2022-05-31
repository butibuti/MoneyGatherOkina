#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpriteParticleGenerator;
	class WaveManager;
	class PauseManagerComponent;

	class FlockingLeader :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "FlockingLeader";
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

		void TutorialStart()
		{
			m_isTutorialStart = true;
			m_isGather = false;
		}

		float GetMoveSpeed() { return (m_pos - m_prevPos).GetLength(); }
		bool IsGather() { return m_isGather; }
	private:
		void GatherStart();
		void DiffusionStart();
		//拡散エフェクト
		void CreateCircleFlash();
		//収縮エフェクト
		void CreateStarFlash();

		void TutorialEnemySpawn();

		bool IsStop();

		Value_weak_ptr<WaveManager> m_vwp_waveManager;
		Value_weak_ptr<PauseManagerComponent> m_vwp_pauseManager;

		Vector3 m_prevPos;
		Vector3 m_pos;

		bool m_isDraw;
		bool m_isGather;

		bool m_isTutorial;
		bool m_isTutorialStart;
		bool m_isTutorialEnemySpawn;
		
		Value_weak_ptr<SpriteParticleGenerator> m_vwp_spriteParticleGenerator;
	};

}

BUTI_REGIST_GAMECOMPONENT(FlockingLeader, true);