#include "stdafx_u.h"
#include "WaveManager.h"
#include "StartPopUpComponent.h"
#include "EnemySpawnManager.h"
#include "Player.h"
#include "InputManager.h"
#include "EnemySpawner.h"
#include "SceneChangeAnimationComponent.h"
#include "GameOverManagerComponent.h"

void ButiEngine::WaveManager::OnUpdate()
{
	if (!m_isSceneStart)
	{
		m_isSceneStart = true;
		m_vwp_sceneChangeAnimationComponent.lock()->SceneStart();
	}

	MoveWave();

	if (m_isWaveTime)
	{
		//�E�F�[�u��

		if (m_vwp_playerComponent.lock()->IsDead())
		{
			m_isGameOver = true;
		}

	}
	else
	{
		//�E�F�[�u�O

		//�ŏI�E�F�[�u���N���A���Ă�����
		if (m_waveNum >= m_maxWaveNum)
		{
			m_isLastWaveClear = true;
		}

		if (m_isPopupSpawn && !m_isLastWaveClear)
		{
			m_isPopupSpawn = false;
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
	m_vwp_playerComponent = GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<Player>();
	auto sceneChangeAnimation = GetManager().lock()->AddObjectFromCereal("SceneChangeAnimation");
	m_vwp_sceneChangeAnimationComponent = sceneChangeAnimation.lock()->GetGameComponent<SceneChangeAnimationComponent>();
	m_waveNum = 0;
	m_maxWaveNum = 1;
	m_clearAnimationTime = 0;
	m_isWaveTime = false;
	m_isPopupSpawn = false;
	m_isLastWaveClear = false;
	m_isGameOver = false;
	m_isNextSceneButton = false;
	m_isGameOverButton = false;
	m_isNextScene = false;
	m_isSceneStart = false;

	m_enemyDeadCount = 0;
	m_maxEnemyCount = 10;
	m_enemySpawnCount = 0;
}

void ButiEngine::WaveManager::OnShowUI()
{
	GUI::BulletText("WaveNum");
	GUI::InputInt("##waveNum", m_waveNum);
	GUI::BulletText("MaxWaveNum");
	GUI::InputInt("##maxWaveNum", m_maxWaveNum);
	GUI::BulletText("MaxEnemyCount");
	GUI::InputInt("##maxEnemyCount", m_maxEnemyCount);
}

void ButiEngine::WaveManager::WaveStart()
{
	m_isWaveTime = true;
}

void ButiEngine::WaveManager::AddEnemyDeadCount()
{
	m_enemyDeadCount++;
}

void ButiEngine::WaveManager::AddSpawnCount()
{
	m_enemySpawnCount++;
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
	if (m_isLastWaveClear) return;

	//�v���C���[���|�b�v�ɐG�ꂽ��E�F�[�u�J�n
	if (m_vwp_startPopUpObject.lock()->IsHitPlayerFlag() && !m_isWaveTime)
	{
		//�E�F�[�u�ԍ���i�߂�
		m_waveNum++;
		FixWaveNum();

		//�E�F�[�u�J�n
		m_isWaveTime = true;

		//�|�b�v���������߂�
		m_vwp_startPopUpObject.lock()->DisappearPopUp();

		//�G���X�|�[��������
		SpawnEnemy();
	}

	
	////���Ń{�^����5�񉟂�����E�F�[�u���N���A�ł���悤�ɂ��Ă���
	//if (InputManager::IsTriggerCancelKey() && m_isWaveTime)
	//{
	//	m_enemyCount++;
	//}
	//�t�B�[���h���̓G�����ׂē|���Ă�����
	if (m_enemyDeadCount >= m_maxEnemyCount && m_isWaveTime)
	{
		WaveFinish();
	}
}

void ButiEngine::WaveManager::SpawnEnemy()
{
	////�E�F�[�u�ԍ��ɉ����ďo��������G�̃p�^�[����z�u��ς���
	////���X�e�[�W�ځE���E�F�[�u�ځE�G�̖��O�E�ʒu

	//auto sceneName = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
	//std::int16_t underScoreIndex = sceneName.find("_");
	//auto size = sceneName.size();
	//std::string stageNum = sceneName.substr(underScoreIndex + 1, size);

	//std::string fileName = "EnemyData/0_" /*+ stageNum + "_"*/ + std::to_string(m_waveNum) + ".enemyData";

	//std::vector<EnemySpawnData> vec_enemySpawnDatas;
	//InputCereal(vec_enemySpawnDatas, fileName);

	//for (auto enemyData : vec_enemySpawnDatas)
	//{
	//	auto enemy = GetManager().lock()->AddObjectFromCereal(enemyData.m_enemyName);
	//	auto transformData = enemyData.m_vlp_enemyTransform;
	//	enemy.lock()->transform->SetLocalPosition(transformData->GetLocalPosition());
	//	enemy.lock()->transform->SetLocalRotation(transformData->GetLocalRotation());

	//	m_enemyCount++;
	//}


	//�X�|�i�[�𐶐�
	auto enemy0 = GetManager().lock()->AddObjectFromCereal("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();
	enemy0->SetType(0);
	auto enemy1 = GetManager().lock()->AddObjectFromCereal("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();
	enemy1->SetType(1);
	auto enemy2 = GetManager().lock()->AddObjectFromCereal("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();
	enemy2->SetType(2);
	auto enemy3 = GetManager().lock()->AddObjectFromCereal("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();
	enemy3->SetType(3);

}

void ButiEngine::WaveManager::WaveFinish()
{
	//�E�F�[�u�I��
	m_isWaveTime = false;
	m_isPopupSpawn = true;
	m_enemyDeadCount = 0;
}

void ButiEngine::WaveManager::StageClearAnimation()
{
	//�ŏI�E�F�[�u�N���A��ɒʂ�悤�ɂ���
	if (!m_isLastWaveClear) return;

	//��
	if (m_clearAnimationTime < 120)
	{
		m_clearAnimationTime++;
	}
	else
	{
		m_isNextSceneButton = true;
	}

	//�X�e�[�W�Z���N�g��
	if (InputManager::IsTriggerDecideKey() && m_isNextSceneButton)
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
	if (!m_isGameOver) return;

	if (!m_vwp_gameOverManagerComponent.lock())
	{
		auto gameOverManager = GetManager().lock()->AddObjectFromCereal("GameOverManager");
		m_vwp_gameOverManagerComponent = gameOverManager.lock()->GetGameComponent<GameOverManagerComponent>();
	}

	if (InputManager::IsTriggerDecideKey() && m_vwp_gameOverManagerComponent.lock()->IsNext())
	{
		m_vwp_sceneChangeAnimationComponent.lock()->SceneEnd();
		m_isNextScene = true;
	}
	else if (!m_vwp_sceneChangeAnimationComponent.lock()->IsAnimation() && m_isNextScene)
	{
		//���g���C���^�C�g����
		if (m_vwp_gameOverManagerComponent.lock()->IsRetry())
		{
			auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
			std::string sceneName = "GamePlay";
			sceneManager->RemoveScene(sceneName);
			sceneManager->LoadScene(sceneName);
			sceneManager->ChangeScene(sceneName);
		}
		else
		{
			auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
			std::string sceneName = "Title";
			sceneManager->RemoveScene(sceneName);
			sceneManager->LoadScene(sceneName);
			sceneManager->ChangeScene(sceneName);
		}

		////�E�F�[�u�̓r�������蒼��
		//m_waveNum--;
		//m_isGameOver = false;
		//m_vwp_playerComponent.lock()->Revival(); //�v���C���[�h��
		//WaveFinish();
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WaveManager::Clone()
{
	return ObjectFactory::Create<WaveManager>();
}
