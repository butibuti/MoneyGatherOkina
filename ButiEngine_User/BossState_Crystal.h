#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpriteParticleGenerator;

	enum class CrystalPhase
	{
		Charge,
		Appear
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

		void CreateObjectAround(const std::string& arg_objectName);

		void SetPhaseParameter();

		CrystalPhase m_phase;

		Value_ptr<Timer> m_vlp_chargeTimer;

		bool m_isStrengthened;

		Value_weak_ptr<SpriteParticleGenerator> m_vwp_spriteParticleGenerator;
	};

}

BUTI_REGIST_GAMECOMPONENT(BossState_Crystal, true);