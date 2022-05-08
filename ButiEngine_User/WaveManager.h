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
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void WaveStart();
		bool IsClearAnimationFlag();

	private:
		void FixWaveNum();
		void MoveWave();
		void SpawnEnemy();
		void WaveFinish();
		void StageClearAnimation();
		void GameOverAnimation();

		Value_weak_ptr<StartPopUpComponent> m_vwp_startPopUpObject;

		//�E�F�[�u�ԍ�
		std::int32_t m_waveNum;
		std::int32_t m_maxWaveNum;

		std::int32_t m_clearAnimationTime;
		std::int32_t m_gameOverAnimationTime;

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

		//���̃E�F�[�u�ɐi�ނ܂ł̉��̃J�E���g
		std::int8_t m_nextWaveCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(WaveManager, true);
