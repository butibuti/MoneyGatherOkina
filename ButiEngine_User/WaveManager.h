#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class StartPopUpComponent;
	class Player;
	class SceneChangeAnimationComponent;
	class GameOverManagerComponent;

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
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void WaveStart();
		void SubEnemyDeadCount();
		bool IsClearAnimationFlag();

	private:
		void FixWaveNum();
		void MoveWave();
		void SpawnEnemy();
		void WaveFinish();
		void StageClearAnimation();
		void GameOverAnimation();

		Value_weak_ptr<StartPopUpComponent> m_vwp_startPopUpObject;
		Value_weak_ptr<Player> m_vwp_playerComponent;
		Value_weak_ptr<SceneChangeAnimationComponent> m_vwp_sceneChangeAnimationComponent;
		Value_weak_ptr<GameOverManagerComponent> m_vwp_gameOverManagerComponent;

		//ウェーブ番号
		std::int32_t m_waveNum;
		std::int32_t m_maxWaveNum;

		std::int32_t m_clearAnimationTime;

		//Wave中かどうか
		bool m_isWaveTime; 
		//ポップを出現させるためのフラグ
		bool m_isPopupSpawn;
		//最終ウェーブをクリアしているかどうか
		bool m_isLastWaveClear;
		//ゲームオーバーかどうか
		bool m_isGameOver;
		//次のシーンにいくためのボタンが押せる状態かどうか
		bool m_isNextSceneButton;
		//ゲームオーバー時のリトライボタン用
		bool m_isGameOverButton;
		bool m_isNextScene;
		bool m_isSceneStart;

		//次のウェーブに進むまでの仮のカウント
		std::int8_t m_enemyCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(WaveManager, true);
