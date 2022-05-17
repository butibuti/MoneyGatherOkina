#include "stdafx_u.h"
#include "EnemySpawner.h"
#include "WaveManager.h"

void ButiEngine::EnemySpawner::OnUpdate()
{
	//�X�V�����ɓ������^�C�~���O�ň�x�����ʂ���������
	if (!m_isOnce)
	{
		m_isOnce = true;
		OnceUpdate();
	}

	//�o���J�n�܂őҋ@
	if (!m_vlp_waitTimer->Update_continue())
	{
		return;
	}

	//�o���Ԋu�����Ԍo�߂ɉ����ĒZ�����Ă���
	if(m_vlp_subWaitTimer->Update())
	{
		if (m_spawnRate > m_maxSpawnRate)
		{
			m_spawnRate--;
			m_vlp_spawnTimer->ChangeCountFrame(m_spawnRate);
		}	
	}

	//��莞�Ԃ�������G���X�|�[��������
	if (m_vlp_spawnTimer->Update())
	{
		SpawnEnemy();
	}
}

void ButiEngine::EnemySpawner::OnSet()
{
	m_vlp_spawnTimer = ObjectFactory::Create<RelativeTimer>();
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>();
	m_vlp_subWaitTimer = ObjectFactory::Create<RelativeTimer>();
}

void ButiEngine::EnemySpawner::Start()
{
	m_vlp_spawnTimer->Start();
	m_vlp_waitTimer->Start();
	m_vlp_subWaitTimer->Start();
	m_vlp_subWaitTimer->ChangeCountFrame(10);
	m_waveManagerComponent = GetManager().lock()->GetGameObject("WaveManager").lock()->GetGameComponent<WaveManager>();
	m_spawnRate = 0;
	m_maxSpawnRate = 0;
	m_isOnce = false;
}

void ButiEngine::EnemySpawner::OnShowUI()
{
	switch (m_spawnType)
	{
	case 0: //�n�G
		GUI::BulletText("FlySpawner");
		break;
	case 1: //�X�g�[�J�[
		GUI::BulletText("StalkerSpawner");
		break;
	case 2: //�L�o
		GUI::BulletText("KibaSpawner");
		break;
	case 3: //�J�U��
		GUI::BulletText("VolcanoSpawner");
		break;
	default:
		break;
	}
	
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EnemySpawner::Clone()
{
	return ObjectFactory::Create<EnemySpawner>();
}

void ButiEngine::EnemySpawner::SetType(const std::int8_t arg_num)
{
	m_spawnType = arg_num;
}

void ButiEngine::EnemySpawner::OnceUpdate()
{
	//�o�����n�߂�܂łɂǂ̂��炢�҂�
	std::int32_t waitFrame = 1;

	switch (m_spawnType)
	{
	case 0: //�n�G
		m_spawnRate = 360;
		m_maxSpawnRate = 360;
		waitFrame = 60;
		break;
	case 1: //�X�g�[�J�[
		m_spawnRate = 1200;
		m_maxSpawnRate = 960;
		waitFrame = 1020;
		break;
	case 2: //�L�o
		m_spawnRate = 900;
		m_maxSpawnRate = 780;
		waitFrame = 1920;
		break;
	case 3: //�J�U��
		m_spawnRate = 1200;
		m_maxSpawnRate = 1020;
		waitFrame = 3000;
		break;
	default:
		m_spawnRate = 360;
		m_maxSpawnRate = 360;
		waitFrame = 1;
		break;
	}

	m_vlp_spawnTimer->ChangeCountFrame(m_spawnRate);
	m_vlp_waitTimer->ChangeCountFrame(waitFrame);
}

void ButiEngine::EnemySpawner::SpawnEnemy()
{
	//�Z�̏o�����Ă�����X�|�[�������Ȃ�
	if (m_waveManagerComponent.lock()->GetSpawnCount() >= m_waveManagerComponent.lock()->GetMaxEnemyCount()) { return; }

	//�G��ǉ�������J�E���g�𑝂₷
	m_waveManagerComponent.lock()->AddSpawnCount();

	//�o���ʒu�������߂�(�����_��)
	Vector3 randomPosition = Vector3(0, 0, 0);
	randomPosition.x = (float)ButiRandom::GetInt(-100, 100);
	randomPosition.z = (float)ButiRandom::GetInt(-100, 100);
	randomPosition.Normalize();
	randomPosition *= (float)ButiRandom::GetInt(1, 20);

	float randomRotateY = 0;
	randomRotateY = (float)ButiRandom::GetInt(-180, 180);

	Value_weak_ptr<GameObject> enemy;

	//�X�|�[��������
	switch (m_spawnType)
	{
	case 0: //�n�G
		enemy = GetManager().lock()->AddObjectFromCereal("Enemy_Fly");
		break;
	case 1: //�X�g�[�J�[
		enemy = GetManager().lock()->AddObjectFromCereal("Enemy_Stalker");
		break;
	case 2: //�L�o
		enemy = GetManager().lock()->AddObjectFromCereal("Enemy_Kiba");
		break;
	case 3: //�J�U��
		enemy = GetManager().lock()->AddObjectFromCereal("Enemy_Volcano");
		break;
	default:
		break;
	}

	enemy.lock()->transform->SetLocalPosition(randomPosition);
	enemy.lock()->transform->SetLocalRotationY_Degrees(randomRotateY);
}
