#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class AttackFlashSpawner :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "AttackFlashSpawner";
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

		void SetSpawnIntervalFrame(const std::int32_t arg_intervalFrame)
		{
			m_spawnIntervalFrame = arg_intervalFrame;
			m_vlp_spawnIntervalTimer->ChangeCountFrame(m_spawnIntervalFrame);
		}

		void SetSpawnRadius(const float arg_radius) { m_spawnRadius = arg_radius; }
		void SetEffectScale(const Vector3& arg_scale) { m_effectScale = arg_scale; }

		void Dead();
		void SpawnStart(const Vector3& arg_pos);
		void SpawnStop();
	private:
		void SpawnAttackFlash();

		Value_ptr<Timer> m_vlp_spawnIntervalTimer;

		float m_spawnRadius;
		Vector3 m_effectScale;
		std::int32_t m_spawnIntervalFrame;
	};

}

BUTI_REGIST_GAMECOMPONENT(AttackFlashSpawner, true);