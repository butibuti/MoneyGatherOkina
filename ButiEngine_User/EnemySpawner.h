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

		Value_weak_ptr<WaveManager> m_waveManagerComponent;
		Value_ptr<Timer> m_vlp_spawnTimer;
		Value_ptr<Timer> m_vlp_waitTimer;
		Value_ptr<Timer> m_vlp_subWaitTimer;
		
		std::int8_t m_spawnType;
		std::int32_t m_spawnRate;
		std::int32_t m_maxSpawnRate;

		bool m_isOnce;
	};
}

BUTI_REGIST_GAMECOMPONENT(EnemySpawner, true);

