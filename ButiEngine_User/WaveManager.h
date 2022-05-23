#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class StartPopUpComponent;
	class Player;
	class SceneChangeAnimationComponent;
	class GameOverManagerComponent;
	class StageClearManagerComponent;
	class StageProgressUIComponent;
	class PauseManagerComponent;

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

		void AddEnemyDeadCount();
		void AddSpawnCount();

		bool IsClearAnimation() { return m_isClear; }
		bool IsGameOver() { return m_isGameOver; }
		std::int32_t GetSpawnCount() { return m_enemySpawnCount; }
		std::int32_t GetMaxEnemyCount() { return m_maxEnemyCount; }
		std::int32_t GetNowEnemyCount() { return m_enemySpawnCount - m_enemyDeadCount; }

	private:
		void SpawnEnemySpawner();
		void StageClearAnimation();
		void GameOverAnimation();
		void PauseAnimation();
		void StageProgressAnimation();

		Value_weak_ptr<Player> m_vwp_playerComponent;
		Value_weak_ptr<SceneChangeAnimationComponent> m_vwp_sceneChangeAnimationComponent;
		Value_weak_ptr<GameOverManagerComponent> m_vwp_gameOverManagerComponent;
		Value_weak_ptr<StageClearManagerComponent> m_vwp_stageClearManagerComponent;
		Value_weak_ptr<StageProgressUIComponent> m_vwp_stageProgressUIComponent;
		Value_weak_ptr<PauseManagerComponent> m_vwp_pauseManagerComponent;

		std::string m_sceneName;

		//クリアしているかどうか
		bool m_isClear;
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
		////現在フィールドにどれだけいるかのカウント
		//std::int32_t m_nowEnemyCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(WaveManager, true);
