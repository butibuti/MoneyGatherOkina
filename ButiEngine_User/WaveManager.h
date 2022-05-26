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
	class WorldSpeedManager;

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

		void AddProgressPoint(const std::int32_t arg_progressPoint);
		void AddSpawnCount();

		bool IsClearAnimation() { return m_isClear; }
		bool IsGameOver() { return m_isAdvanceGameOver; }
		std::int32_t GetSpawnCount() { return m_enemySpawnCount; }
		std::int32_t GetMaxEnemyCount() { return m_clearPoint; }
		std::int32_t GetNowEnemyCount() { return m_enemySpawnCount - m_progressPoint; }

	private:
		void SpawnEnemySpawner();
		void StageClearAnimation();
		void GameOverAnimation();
		void PauseAnimation();
		void StageProgressAnimation();

		Value_ptr<Timer> m_vlp_advanceGameOverTimer;

		Value_weak_ptr<Player> m_vwp_playerComponent;
		Value_weak_ptr<SceneChangeAnimationComponent> m_vwp_sceneChangeAnimationComponent;
		Value_weak_ptr<GameOverManagerComponent> m_vwp_gameOverManagerComponent;
		Value_weak_ptr<StageClearManagerComponent> m_vwp_stageClearManagerComponent;
		Value_weak_ptr<StageProgressUIComponent> m_vwp_stageProgressUIComponent;
		Value_weak_ptr<PauseManagerComponent> m_vwp_pauseManagerComponent;
		Value_weak_ptr<WorldSpeedManager> m_vwp_worldSpeedManagerComponent;

		std::string m_sceneName;

		//クリアしているかどうか
		bool m_isClear;
		//ゲームオーバーかどうか
		bool m_isGameOver;
		bool m_isAdvanceGameOver;
		bool m_isAdvanceNextGameOver;
		//次のシーンにいくためのボタンが押せる状態かどうか
		bool m_isNextSceneButton;
		//ゲームオーバー時のリトライボタン用
		bool m_isGameOverButton;
		bool m_isNextScene;
		bool m_isSceneStart;

		//ステージクリアまでの敵討伐カウント
		std::int32_t m_progressPoint;
		//ステージクリアまでの敵討伐カウント最大値
		std::int32_t m_clearPoint;
		//〇体まで出現させるためのカウント
		std::int32_t m_enemySpawnCount;
		////現在フィールドにどれだけいるかのカウント
		//std::int32_t m_nowEnemyCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(WaveManager, true);
