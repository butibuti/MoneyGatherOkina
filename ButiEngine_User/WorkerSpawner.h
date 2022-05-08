#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class WorkerSpawner :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "WorkerSpawner";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void StartTimer()
		{
			if (m_vlp_spawnTimer)
			{
				m_vlp_spawnTimer->Start();
			}
		}
	private:
		void SpawnWorker();

		Value_ptr<Timer> m_vlp_spawnTimer;
		std::uint8_t m_spawnIntervalFrame;
	};

}

BUTI_REGIST_GAMECOMPONENT(WorkerSpawner, true);