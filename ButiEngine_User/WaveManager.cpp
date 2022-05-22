#include "stdafx_u.h"
#include "WaveManager.h"
#include "EnemySpawnManager.h"
#include "Player.h"
#include "InputManager.h"
#include "EnemySpawner.h"
#include "SceneChangeAnimationComponent.h"
#include "GameOverManagerComponent.h"
#include "StageClearManagerComponent.h"
#include "StageProgressUIComponent.h"
#include "PauseManagerComponent.h"
#include "WorldSpeedManager.h"

void ButiEngine::WaveManager::OnUpdate()
{
	if (!m_isSceneStart)
	{
		m_isSceneStart = true;
		m_vwp_sceneChangeAnimationComponent.lock()->SceneStart();
	}

	if (m_vwp_playerComponent.lock()->IsDead())
	{
		m_isGameOver = true;
	}
	//�N���A���Ă��邩
	if (m_enemyDeadCount >= m_maxEnemyCount)
	{
		m_isClear = true;
	}

	StageClearAnimation();
	GameOverAnimation();
	PauseAnimation();
	StageProgressAnimation();
}

void ButiEngine::WaveManager::OnSet()
{
	auto oneFrameObject = GetManager().lock()->GetGameObject("DrawObject_OneFrame");
	if (!oneFrameObject.lock())
	{
		oneFrameObject = GetManager().lock()->AddObjectFromCereal("DrawObject_OneFrame");
	}
}

void ButiEngine::WaveManager::Start()
{
	m_vwp_playerComponent = GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<Player>();
	auto sceneChangeAnimation = GetManager().lock()->AddObjectFromCereal("SceneChangeAnimation");
	m_vwp_sceneChangeAnimationComponent = sceneChangeAnimation.lock()->GetGameComponent<SceneChangeAnimationComponent>();
	m_vwp_stageProgressUIComponent = GetManager().lock()->AddObjectFromCereal("StageProgressUI_Inline").lock()->GetGameComponent<StageProgressUIComponent>();
	m_vwp_pauseManagerComponent = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManagerComponent>();
	m_isClear = false;
	m_isGameOver = false;
	m_isNextSceneButton = false;
	m_isGameOverButton = false;
	m_isNextScene = false;
	m_isSceneStart = false;

	m_enemyDeadCount = 0;
	m_maxEnemyCount = 30;
	m_enemySpawnCount = 0;

	//�G�l�~�[�X�|�i�[���X�|�[��������
	SpawnEnemySpawner();
}

void ButiEngine::WaveManager::OnShowUI()
{
	GUI::BulletText("MaxEnemyCount");
	GUI::InputInt("##maxEnemyCount", m_maxEnemyCount);
}

void ButiEngine::WaveManager::AddEnemyDeadCount()
{
	m_enemyDeadCount++;
}

void ButiEngine::WaveManager::AddSpawnCount()
{
	m_enemySpawnCount++;
}

void ButiEngine::WaveManager::SpawnEnemySpawner()
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

void ButiEngine::WaveManager::StageClearAnimation()
{
	//�N���A���ɒʂ�悤�ɂ���
	if (!m_isClear) return;

	if (!m_vwp_stageClearManagerComponent.lock())
	{
		auto stageClearManager = GetManager().lock()->AddObjectFromCereal("StageClearManager");
		m_vwp_stageClearManagerComponent = stageClearManager.lock()->GetGameComponent<StageClearManagerComponent>();
	}

	//�X�e�[�W�Z���N�g��
	if (InputManager::IsTriggerDecideKey() && m_vwp_stageClearManagerComponent.lock()->IsNext())
	{
		m_vwp_sceneChangeAnimationComponent.lock()->SceneEnd();
		m_isNextScene = true;
	}
	else if (!m_vwp_sceneChangeAnimationComponent.lock()->IsAnimation() && m_isNextScene)
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

void ButiEngine::WaveManager::PauseAnimation()
{
	//�|�[�Y���̂ݒʂ�悤�ɂ���
	if (!m_vwp_pauseManagerComponent.lock()->IsPause()) { return; }

	if (InputManager::IsTriggerDecideKey() && m_vwp_pauseManagerComponent.lock()->IsNext())
	{
		if (!m_vwp_pauseManagerComponent.lock()->IsBack())
		{
			m_vwp_sceneChangeAnimationComponent.lock()->SceneEnd();
			m_isNextScene = true;
		}
	}
	else if (!m_vwp_sceneChangeAnimationComponent.lock()->IsAnimation() && m_isNextScene)
	{
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		std::string sceneName = "StageSelect";
		sceneManager->RemoveScene(sceneName);
		sceneManager->LoadScene(sceneName);
		sceneManager->ChangeScene(sceneName);

		GetManager().lock()->GetGameObject("WorldSpeedManager").lock()->GetGameComponent<WorldSpeedManager>()->SetSpeed(1.0f);
	}
}

void ButiEngine::WaveManager::StageProgressAnimation()
{
	if (!m_vwp_stageProgressUIComponent.lock()) { return; }

	auto rate = (float)m_enemyDeadCount / (float)m_maxEnemyCount;
	m_vwp_stageProgressUIComponent.lock()->SetRate(rate);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WaveManager::Clone()
{
	return ObjectFactory::Create<WaveManager>();
}
