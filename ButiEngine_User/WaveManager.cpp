#include "stdafx_u.h"
#include "WaveManager.h"
#include "StartPopUpComponent.h"
#include "InputManager.h"

void ButiEngine::WaveManager::OnUpdate()
{
	MoveWave();

	if (m_waveTimeFlag)
	{
		//�E�F�[�u��
	}
	else
	{
		//�E�F�[�u�O
	}
}

void ButiEngine::WaveManager::OnSet()
{
}

void ButiEngine::WaveManager::Start()
{
	m_vwp_startPopUpObject = GetManager().lock()->GetGameObject("StartPopUpObject").lock()->GetGameComponent<StartPopUpComponent>();

	m_waveNum = 1;
	m_maxWaveNum = 20;
	m_waveTimeFlag = false;

	m_nextWaveCount = 0;
}

void ButiEngine::WaveManager::OnShowUI()
{
}

void ButiEngine::WaveManager::WaveStart()
{
	m_waveTimeFlag = true;
}

void ButiEngine::WaveManager::FixWaveNum()
{
	//Wave�ԍ���͈͓��ɏC��
	if (m_waveNum < 1)
	{
		m_waveNum = 1;
	}
	else if (m_waveNum > m_maxWaveNum)
	{
		m_waveNum = m_maxWaveNum;
	}
}

void ButiEngine::WaveManager::MoveWave()
{
	//�v���C���[���|�b�v�ɐG�ꂽ��E�F�[�u�J�n
    //if (�|�b�v->�v���C���[���G�ꂽ���ǂ���)
	if (m_vwp_startPopUpObject.lock()->IsHitPlayerFlag() && !m_waveTimeFlag)
	//if (InputManager::IsTriggerDecideKey())
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

	if (InputManager::IsTriggerCancelKey() && m_waveTimeFlag)
	{
		m_nextWaveCount++;
	}
	if (m_nextWaveCount >= 5)
	{
		m_nextWaveCount = 0;

		//�E�F�[�u�I��
		m_waveTimeFlag = false;

		//�|�b�v���X�|�[��������
		m_vwp_startPopUpObject.lock()->AppearPopUp();
	}
}

void ButiEngine::WaveManager::SpawnEnemy()
{
	//�E�F�[�u�ԍ��ɉ����ďo��������G�̃p�^�[����z�u��ς���
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WaveManager::Clone()
{
	return ObjectFactory::Create<WaveManager>();
}
