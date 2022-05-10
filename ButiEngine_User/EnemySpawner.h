#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

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

		Vector3 m_randomPosition;
		
		std::int8_t m_spawnType;
		std::int32_t m_spawnTimer;
		std::int32_t m_spawnRate;
		std::int32_t m_maxSpawnRate;
		std::int32_t m_spawnStartTimer;
		std::int16_t m_subWaitTimer;

		bool m_isOnce;
	};
}

BUTI_REGIST_GAMECOMPONENT(EnemySpawner, true);

