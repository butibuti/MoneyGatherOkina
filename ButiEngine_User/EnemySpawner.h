#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class WaveManager;

	class EnemySpawner :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "EnemySpawner";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void SetType(const std::int8_t arg_num);

	private:
		void OnceUpdate();
		void SpawnEnemy();
		void FixShorteningFrame();
		void SetRandomSpawnFrame();

		Value_weak_ptr<WaveManager> m_waveManagerComponent;
		Value_ptr<Timer> m_vlp_spawnTimer;
		Value_ptr<Timer> m_vlp_waitTimer;
		
		std::int8_t m_spawnType;

		float m_startMaxSpawnFrame;
		float m_endMaxSpawnFrame;
		float m_currentMaxSpawnFrame;
		float m_startMinSpawnFrame;
		float m_endMinSpawnFrame;
		float m_currentMinSpawnFrame;
		float m_startWaitFrame;
		float m_lastIntervalReachFrame;
		float m_randomSpawnFrame;

		float m_reachShorteningMaxFrame;
		float m_reachShorteningMinFrame;

		bool m_isOnce;
	};
}

BUTI_REGIST_GAMECOMPONENT(EnemySpawner, true);

