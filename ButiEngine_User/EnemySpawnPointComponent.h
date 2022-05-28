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
		void SetEnemyTag(const std::string& arg_enemyTag) { m_enemyTag = arg_enemyTag; }
		void SetEnemyObject(Value_weak_ptr<GameObject> arg_vwp_enemy);
		Value_weak_ptr<GameObject>GetEnemyObject()const {
			return m_vwp_enemyObject;
		}
	private:
		Value_ptr<Timer> m_vlp_spawnAnimationTimer,m_vlp_brightOffTimer;
		Value_weak_ptr<GameObject> m_vlp_diamond,m_vwp_appearnceObject,m_vwp_enemyObject,m_vwp_enemyDrawObject;
		List<Value_weak_ptr<ButiRendering::CBuffer< ButiRendering::ObjectInformation>>> m_list_appearanceDrawInformation, m_list_drawObjectDrawInformation;
		Vector3 m_position;
		float m_diamondTargetScale,m_enemyDirection,m_effectProgress,m_effectTargetScale;
		std::int8_t m_spawnType;
		std::string m_enemyTag;
	};
}

BUTI_REGIST_GAMECOMPONENT(EnemySpawnPointComponent, true);

