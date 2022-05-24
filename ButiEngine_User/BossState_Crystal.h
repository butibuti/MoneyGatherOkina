#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpriteParticleGenerator;

	enum class CrystalStatePhase
	{
		Charge,
		Appear,
		Wait
	};

	class BossState_Crystal :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "BossState_Crystal";
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

		void EndState();
	private:
		void Charge();
		void ChargeStart();
		void Appear();
		void AppearStart();
		void Wait();
		void WaitStart();

		void CreateObjectAround(const std::string& arg_objectName);

		void SetPhaseParameter();

		CrystalStatePhase m_phase;

		Value_ptr<Timer> m_vlp_chargeTimer;
		Value_ptr<Timer> m_vlp_appearTimer;
		Value_ptr<Timer> m_vlp_waitTimer;
		Value_weak_ptr<MeshDrawComponent> m_vwp_screenMeshDraw;

		Vector4 m_defaultBloomPower;
		Vector4 m_maxBloomPower;

		bool m_isStrengthened;

		Value_weak_ptr<SpriteParticleGenerator> m_vwp_spriteParticleGenerator;
	};

}

BUTI_REGIST_GAMECOMPONENT(BossState_Crystal, true);