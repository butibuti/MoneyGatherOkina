#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class WaveManager;

	class EnemySpawnPointComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "EnemySpawnPointComponent";
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
		void SetPosition(const Vector3& arg_position) { m_position = arg_position; }

	private:
		Value_ptr<Timer> m_vlp_spawnAnimationTimer;
		Value_weak_ptr<GameObject> m_vlp_diamond;

		Vector3 m_position;
		Vector3 m_scale;
		float m_moveScale;
		std::int8_t m_spawnType;
	};
}

BUTI_REGIST_GAMECOMPONENT(EnemySpawnPointComponent, true);

