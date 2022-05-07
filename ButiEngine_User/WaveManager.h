#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class StartPopUpComponent;

	class WaveManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "WaveManager";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		void WaveStart();
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		void FixWaveNum();
		void MoveWave();
		void SpawnEnemy();

		Value_weak_ptr<StartPopUpComponent> m_vwp_startPopUpObject;

		//ウェーブ番号
		std::int8_t m_waveNum;
		std::int8_t m_maxWaveNum;

		//Wave中かどうか
		bool m_waveTimeFlag; 

		//次のウェーブに進むまでの仮のカウント
		std::int8_t m_nextWaveCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(WaveManager, true);
