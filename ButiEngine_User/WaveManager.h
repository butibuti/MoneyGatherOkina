#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class StartPopUpComponent;
	class Player;
	class SceneChangeAnimationComponent;
	class GameOverManagerComponent;
	class StageProgressUIComponent;

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
		void AddEnemyDeadCount();
		void AddSpawnCount();

		bool IsClearAnimationFlag() { return m_isLastWaveClear; }
		std::int32_t GetSpawnCount() { return m_enemySpawnCount; }
		std::int32_t GetMaxEnemyCount() { return m_maxEnemyCount; }

	private:
		void FixWaveNum();
		void MoveWave();
		void SpawnEnemy();
		void WaveFinish();
		void StageClearAnimation();
		void GameOverAnimation();
		void StageProgressAnimation();

		Value_weak_ptr<StartPopUpComponent> m_vwp_startPopUpObject;
		Value_weak_ptr<Player> m_vwp_playerComponent;
		Value_weak_ptr<SceneChangeAnimationComponent> m_vwp_sceneChangeAnimationComponent;
		Value_weak_ptr<GameOverManagerComponent> m_vwp_gameOverManagerComponent;
		Value_weak_ptr<StageProgressUIComponent> m_vwp_stageProgressUIComponent;

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

		//ステージクリアまでの敵討伐カウント
		std::int32_t m_enemyDeadCount;
		//ステージクリアまでの敵討伐カウント最大値
		std::int32_t m_maxEnemyCount;
		//〇体まで出現させるためのカウント
		std::int32_t m_enemySpawnCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(WaveManager, true);
