#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class StartPopUpComponent;
	class Player;
	class SceneChangeAnimationComponent;
	class GameOverManagerComponent;
	class StageClearManagerComponent;
	class AllStageClearManagerComponent;
	class StageProgressUIComponent;
	class PauseManagerComponent;
	class WorldSpeedManager;
	class SoundPlayerComponent;

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

		void AddPoint(const std::int32_t arg_progressPoint);
		void AddSpawnCount();

		void SetIsTutorial(const bool arg_isTutorial) { m_isTutorial = arg_isTutorial; }

		bool IsTutorial() { return m_isTutorial; }
		bool IsGameStart() { return m_isGameStart; }
		bool IsClearAnimation() { return m_isClear; }
		bool IsGameOver() { return m_isAdvanceGameOver; }
		bool IsEvent() { return m_isClear || m_isAdvanceGameOver; }
		std::int32_t GetSpawnCount() { return m_enemySpawnCount; }
		std::int32_t GetClearPoint() { return m_clearPoint; }
		std::int32_t GetNowEnemyCount() { return m_enemySpawnCount - m_point; }
		std::int32_t GetPoint() { return m_point; }

		static void SetRetryPoint(const std::int32_t arg_retryPoint) { m_retryPoint = arg_retryPoint; }
		static std::int32_t GetRetryPoint() { return m_retryPoint; }

		void SpawnEnemySpawner();
	private:
		void StageClearAnimation();
		void GameOverAnimation();
		void PauseAnimation();
		void StageProgressAnimation();
		void SpawnAnimation();

		Value_ptr<Timer> m_vlp_advanceGameOverTimer;
		Value_ptr<Timer> m_vlp_spawnTimer;
		Value_ptr<Timer> m_vlp_spawnIntervalTimer;

		Value_weak_ptr<Player> m_vwp_playerComponent;
		Value_weak_ptr<SceneChangeAnimationComponent> m_vwp_sceneChangeAnimationComponent;
		Value_weak_ptr<GameOverManagerComponent> m_vwp_gameOverManagerComponent;
		Value_weak_ptr<StageClearManagerComponent> m_vwp_stageClearManagerComponent;
		Value_weak_ptr<AllStageClearManagerComponent> m_vwp_allStageClearManagerComponent;
		Value_weak_ptr<StageProgressUIComponent> m_vwp_stageProgressUIComponent;
		Value_weak_ptr<PauseManagerComponent> m_vwp_pauseManagerComponent;
		Value_weak_ptr<WorldSpeedManager> m_vwp_worldSpeedManagerComponent;
		Value_weak_ptr<SoundPlayerComponent> m_vwp_soundPlayerComponent;

		std::string m_sceneName;

		bool m_isTutorial;
		bool m_isGameStart;

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
		bool m_isSpawnAnimation;
		bool m_isSpawnedPlayer;
		bool m_isSpawnWorker;

		//現在のポイント
		std::int32_t m_point;
		//ステージクリアに必要なポイント
		std::int32_t m_clearPoint;
		//〇体まで出現させるためのカウント
		std::int32_t m_enemySpawnCount;
		////現在フィールドにどれだけいるかのカウント
		//std::int32_t m_nowEnemyCount;

		static std::int32_t m_tutorialClearPoint;
		static std::int32_t m_stageClearPoint;
		static std::int32_t m_retryPoint;
	};

}

BUTI_REGIST_GAMECOMPONENT(WaveManager, true);
