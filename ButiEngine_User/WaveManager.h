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

		//�E�F�[�u�ԍ�
		std::int32_t m_waveNum;
		std::int32_t m_maxWaveNum;

		std::int32_t m_clearAnimationTime;

		//Wave�����ǂ���
		bool m_isWaveTime; 
		//�|�b�v���o�������邽�߂̃t���O
		bool m_isPopupSpawn;
		//�ŏI�E�F�[�u���N���A���Ă��邩�ǂ���
		bool m_isLastWaveClear;
		//�Q�[���I�[�o�[���ǂ���
		bool m_isGameOver;
		//���̃V�[���ɂ������߂̃{�^�����������Ԃ��ǂ���
		bool m_isNextSceneButton;
		//�Q�[���I�[�o�[���̃��g���C�{�^���p
		bool m_isGameOverButton;
		bool m_isNextScene;
		bool m_isSceneStart;

		//�X�e�[�W�N���A�܂ł̓G�����J�E���g
		std::int32_t m_enemyDeadCount;
		//�X�e�[�W�N���A�܂ł̓G�����J�E���g�ő�l
		std::int32_t m_maxEnemyCount;
		//�Z�̂܂ŏo�������邽�߂̃J�E���g
		std::int32_t m_enemySpawnCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(WaveManager, true);
