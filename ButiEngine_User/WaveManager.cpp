#include "stdafx_u.h"
#include "WaveManager.h"
#include "StartPopUpComponent.h"
#include "EnemySpawnManager.h"
#include "InputManager.h"

void ButiEngine::WaveManager::OnUpdate()
{
	MoveWave();

	if (m_waveTimeFlag)
	{
		//�E�F�[�u��

		if (InputManager::IsTriggerPauseKey())
		{
			m_gameOverFlag = true;
		}

	}
	else
	{
		//�E�F�[�u�O

		//�ŏI�E�F�[�u���N���A���Ă�����
		if (m_waveNum >= m_maxWaveNum)
		{
			m_lastWaveClearFlag = true;
		}

		if (m_popupSpawnFlag && !m_lastWaveClearFlag)
		{
			m_popupSpawnFlag = false;
			//�|�b�v���X�|�[��������
			m_vwp_startPopUpObject.lock()->AppearPopUp();
		}
	}

	StageClearAnimation();
	GameOverAnimation();
}

void ButiEngine::WaveManager::OnSet()
{
}

void ButiEngine::WaveManager::Start()
{
	m_vwp_startPopUpObject = GetManager().lock()->GetGameObject("StartPopUpObject").lock()->GetGameComponent<StartPopUpComponent>();

	m_waveNum = 0;
	m_maxWaveNum = 20;
	m_clearAnimationTime = 0;
	m_gameOverAnimationTime = 0;
	m_waveTimeFlag = false;
	m_popupSpawnFlag = false;
	m_lastWaveClearFlag = false;
	m_gameOverFlag = false;
	m_nextSceneButtonFlag = false;
	m_gameOverButtonFlag = false;

	m_nextWaveCount = 0;
}

void ButiEngine::WaveManager::OnShowUI()
{
	GUI::BulletText("WaveNum");
	GUI::InputInt("##waveNum", m_waveNum);
}

void ButiEngine::WaveManager::WaveStart()
{
	m_waveTimeFlag = true;
}

bool ButiEngine::WaveManager::IsClearAnimationFlag()
{
	return m_lastWaveClearFlag;
}

void ButiEngine::WaveManager::FixWaveNum()
{
	//Wave�ԍ���͈͓��ɏC��
	if (m_waveNum < 0)
	{
		m_waveNum = 0;
	}
	else if (m_waveNum > m_maxWaveNum)
	{
		m_waveNum = m_maxWaveNum;
	}
}

void ButiEngine::WaveManager::MoveWave()
{
	//�ŏI�E�F�[�u���N���A���Ă�����ʂ�Ȃ��悤�ɂ���
	if (m_lastWaveClearFlag) return;

	//�v���C���[���|�b�v�ɐG�ꂽ��E�F�[�u�J�n
	if (m_vwp_startPopUpObject.lock()->IsHitPlayerFlag() && !m_waveTimeFlag)
	{
		//�E�F�[�u�ԍ���i�߂�
		m_waveNum++;
		FixWaveNum();

		//�E�F�[�u�J�n
		m_waveTimeFlag = true;

		//�|�b�v���������߂�
		m_vwp_startPopUpObject.lock()->DisappearPopUp();

		//�G���X�|�[��������
		SpawnEnemy();
	}

	
	//���Ń{�^����5�񉟂�����E�F�[�u���N���A�ł���悤�ɂ��Ă���
	if (InputManager::IsTriggerCancelKey() && m_waveTimeFlag)
	{
		m_nextWaveCount++;
	}
	//�t�B�[���h���̓G�����ׂē|���Ă�����ɕς���
	if (m_nextWaveCount >= 5)
	{
		m_nextWaveCount = 0;
	
		WaveFinish();
	}
}

void ButiEngine::WaveManager::SpawnEnemy()
{
	//�E�F�[�u�ԍ��ɉ����ďo��������G�̃p�^�[����z�u��ς���
	//���X�e�[�W�ځE���E�F�[�u�ځE�G�̖��O�E�ʒu

	auto sceneName = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
	std::int16_t underScoreIndex = sceneName.find("_");
	auto size = sceneName.size();
	std::string stageNum = sceneName.substr(underScoreIndex + 1, size);

	std::string fileName = "EnemyData/1_" /*+ stageNum + "_"*/ + std::to_string(m_waveNum) + ".enemyData";

	std::vector<EnemySpawnData> vec_enemySpawnDatas;
	InputCereal(vec_enemySpawnDatas, fileName);

	for (auto enemyData : vec_enemySpawnDatas)
	{
		auto enemy = GetManager().lock()->AddObjectFromCereal(enemyData.m_enemyName);
		auto transformData = enemyData.m_vlp_enemyTransform;
		enemy.lock()->transform->SetLocalPosition(transformData->GetLocalPosition());
		enemy.lock()->transform->SetLocalRotation(transformData->GetLocalRotation());
	}
	
}

void ButiEngine::WaveManager::WaveFinish()
{
	//�E�F�[�u�I��
	m_waveTimeFlag = false;
	m_popupSpawnFlag = true;
}

void ButiEngine::WaveManager::StageClearAnimation()
{
	//�ŏI�E�F�[�u�N���A��ɒʂ�悤�ɂ���
	if (!m_lastWaveClearFlag) return;

	//��
	if (m_clearAnimationTime < 120)
	{
		m_clearAnimationTime++;
	}
	else
	{
		m_nextSceneButtonFlag = true;
	}

	//�X�e�[�W�Z���N�g��
	if (InputManager::IsTriggerDecideKey() && m_nextSceneButtonFlag)
	{
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		std::string sceneName = "StageSelect";
		sceneManager->RemoveScene(sceneName);
		sceneManager->LoadScene(sceneName);
		sceneManager->ChangeScene(sceneName);
	}
}

void ButiEngine::WaveManager::GameOverAnimation()
{
	//�Q�[���I�[�o�[�̎��ɒʂ�悤�ɂ���
	if (!m_gameOverFlag) return;

	//��
	if (m_gameOverAnimationTime < 120)
	{
		m_gameOverAnimationTime++;
	}
	else
	{
		m_gameOverButtonFlag = true;
	}

	//�E�F�[�u�̓r�������蒼��
	if (InputManager::IsTriggerDecideKey() && m_gameOverButtonFlag)
	{
		m_waveNum--;
		m_gameOverFlag = false;
		WaveFinish();
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WaveManager::Clone()
{
	return ObjectFactory::Create<WaveManager>();
}
