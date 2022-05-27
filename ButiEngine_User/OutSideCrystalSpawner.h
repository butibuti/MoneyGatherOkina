#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class OutsideCrystal;
	class WaveManager;

	class OutsideCrystalSpawner :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "OutsideCrystalSpawner";
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
	private:
		void ActivePhase(const std::int32_t arg_index);

		void CreateCrystals();

		static std::int32_t m_startCrystalCount;
		static float m_startAngle;
		static float m_spawnRadius;
		static std::int32_t m_phaseCount;
		static std::vector<std::int32_t> m_vec_phaseActivePoints;

		std::vector<std::vector<Value_weak_ptr<OutsideCrystal>>> m_vec_crystals;
		Value_weak_ptr<WaveManager> m_vwp_waveManager;
	};

}

BUTI_REGIST_GAMECOMPONENT(OutsideCrystalSpawner, true);

