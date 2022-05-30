#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Worker;

	class WorkerSpawner :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "WorkerSpawner";
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

		void SpawnStart()
		{
			if (m_isTutorial) { return; }
			m_isSpawnWorker = true;
			m_vlp_spawnTimer->Start();
		}

		void RandomSpawnStart()
		{
			if (!m_isTutorial) { return; }
			m_isRandomSpawnWorker = true;
			m_vlp_randomSpawnTimer->Start();
		}
	private:
		void RandomSpawnWorker();
		void SpawnWorker();
		void SetWorkerPosition();

		Value_ptr<Timer> m_vlp_spawnTimer;
		Value_ptr<Timer> m_vlp_randomSpawnTimer;
		std::vector<Value_weak_ptr<Worker>> m_vec_workers;
		std::int32_t m_spawnIntervalFrame;
		std::int32_t m_initSpawnWorkerCount;

		bool m_isRandomSpawnWorker;
		bool m_isSpawnWorker;
		bool m_isSpawned;
		bool m_isTutorial;
	};

}

BUTI_REGIST_GAMECOMPONENT(WorkerSpawner, true);