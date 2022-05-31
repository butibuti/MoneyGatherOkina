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

		void SetType(const std::int8_t arg_num) { m_spawnType = arg_num; }
		void SetStageNum(const std::string arg_num) { m_stageNumber = arg_num; }
		void SetSpawnerIndex(const std::int8_t arg_index) { m_spawnerIndex = arg_index; }

	private:
		void OnceUpdate();
		void SpawnEnemy();
		void FixShorteningFrame();
		void SetRandomSpawnFrame();
		void SpawnAnimation();

		Value_weak_ptr<WaveManager> m_waveManagerComponent;
		Value_ptr<Timer> m_vlp_spawnTimer;
		Value_ptr<Timer> m_vlp_changeTimer;
		
		std::int32_t m_maxEnemyFieldCount;
		std::int32_t m_maxHeatEnemyFieldCount;
		std::int32_t m_inputStageNumber;
		std::int8_t m_spawnType;
		std::int8_t m_spawnerIndex;
		std::string m_stageNumber;
		std::string m_tag;

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

		float m_maxSpawnRate;
		float m_minSpawnRate;
		float m_changeInterval;

		bool m_isOnce;
		bool m_isHeatUp;
		bool m_isHeatUpActive;
	};
}

BUTI_REGIST_GAMECOMPONENT(EnemySpawner, true);

