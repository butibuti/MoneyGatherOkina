#include "stdafx_u.h"
#include "CameraAxis.h"
#include "InputManager.h"

void ButiEngine::CameraAxis::OnUpdate()
{
	ChasePlayer(0.1f);
	//RotateAxis(0.1f);
}

void ButiEngine::CameraAxis::OnSet()
{
}

void ButiEngine::CameraAxis::OnShowUI()
{
	GUI::BulletText("CurrentRotation");
	GUI::DragFloat3("##currentRotation", m_currentRotation, 0.01f, -180.0f, 180.0f);
}

void ButiEngine::CameraAxis::Start()
{
	//プレイヤーの取得
	m_vwp_player = GetManager().lock()->GetGameObject(GameObjectTag("Player"));

	m_currentPosition = m_vwp_player.lock()->transform->GetLocalPosition();
	m_previousPosition = m_currentPosition;

	m_currentRotation = Vector3(0, 0, 0);
	m_previousRotation = m_currentRotation;
	//竹渕が書いたブルーム用カメラにmainを親としてセットする処理
	GetCamera("BloomSource")->vlp_transform->SetBaseTransform(GetCamera("main")->vlp_transform);
	GetCamera("IsolateEffect")->vlp_transform->SetBaseTransform(GetCamera("main")->vlp_transform);
	GetCamera("IsolateEffectBloom")->vlp_transform->SetBaseTransform(GetCamera("main")->vlp_transform);
}

void ButiEngine::CameraAxis::ChasePlayer(const float arg_lerpScale)
{
	//プレイヤーを追従する
	m_currentPosition = m_vwp_player.lock()->transform->GetLocalPosition();

	//追従の補間(カメラの動きにディレイをかける)
	m_previousPosition.x = m_previousPosition.x * (1.0f - arg_lerpScale) + m_currentPosition.x * arg_lerpScale;
	m_previousPosition.y = m_previousPosition.y * (1.0f - arg_lerpScale) + m_currentPosition.y * arg_lerpScale;
	m_previousPosition.z = m_previousPosition.z * (1.0f - arg_lerpScale) + m_currentPosition.z * arg_lerpScale;

	//ディレイした結果をセットする
	gameObject.lock()->transform->SetLocalPosition(m_previousPosition);
}

void ButiEngine::CameraAxis::RotateAxis(const float arg_lerpScale)
{
	//入力に応じてカメラの軸を回転させる
	m_currentRotation.y += InputManager::GetRightStick().x;

	//追従の補間(カメラの動きにディレイをかける)
	m_previousRotation.x = m_previousRotation.x * (1.0f - arg_lerpScale) + m_currentRotation.x * arg_lerpScale;
	m_previousRotation.y = m_previousRotation.y * (1.0f - arg_lerpScale) + m_currentRotation.y * arg_lerpScale;
	m_previousRotation.z = m_previousRotation.z * (1.0f - arg_lerpScale) + m_currentRotation.z * arg_lerpScale;
	
	//ディレイした結果をセットする
	gameObject.lock()->transform->SetLocalRotation(m_previousRotation);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::CameraAxis::Clone()
{
	return ObjectFactory::Create<CameraAxis>();
}
