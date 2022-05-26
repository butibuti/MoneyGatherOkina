#include "stdafx_u.h"
#include "InputManager.h"
#include<Xinput.h>

const float ButiEngine::InputManager::m_DEADZONE = 0.25f;
bool ButiEngine::InputManager::m_isNotPushKey;
ButiEngine::Vector2 ButiEngine::InputManager::m_previousRightStick;
ButiEngine::Vector2 ButiEngine::InputManager::m_currentRightStick;
ButiEngine::Vector2 ButiEngine::InputManager::m_previousLeftStick;
ButiEngine::Vector2 ButiEngine::InputManager::m_currentLeftStick;

void ButiEngine::InputManager::OnUpdate()
{
	m_previousRightStick = m_currentRightStick;
	m_previousLeftStick = m_currentLeftStick;

	m_currentRightStick = GameDevice::GetInput()->GetRightStick();
	m_currentLeftStick = GameDevice::GetInput()->GetLeftStick();
}

void ButiEngine::InputManager::OnSet()
{
}

void ButiEngine::InputManager::Start()
{
	m_previousLeftStick = Vector2();
	m_currentLeftStick = Vector2();
	m_previousRightStick = Vector2();
	m_currentRightStick = Vector2();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::InputManager::Clone()
{
	return ObjectFactory::Create<InputManager>();
}

bool ButiEngine::InputManager::IsPushRightKey()
{
	return (GameDevice::GetInput()->CheckKey(Keys::D) ||
		GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_RIGHT) ||
		m_currentRightStick.x >= m_DEADZONE ||
		m_currentLeftStick.x >= m_DEADZONE);
}

bool ButiEngine::InputManager::IsPushLeftKey()
{
	return (GameDevice::GetInput()->CheckKey(Keys::A) ||
		GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_LEFT) ||
		m_currentRightStick.x <= -m_DEADZONE ||
		m_currentLeftStick.x <= -m_DEADZONE);
}

bool ButiEngine::InputManager::IsPushUpKey()
{
	return (GameDevice::GetInput()->CheckKey(Keys::W) ||
		GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_UP) ||
		m_currentRightStick.y >= m_DEADZONE ||
		m_currentLeftStick.y >= m_DEADZONE);
}

bool ButiEngine::InputManager::IsPushDownKey()
{
	return (GameDevice::GetInput()->CheckKey(Keys::S) ||
		GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_DOWN) ||
		m_currentRightStick.y <= -m_DEADZONE ||
		m_currentLeftStick.y <= -m_DEADZONE);
}

bool ButiEngine::InputManager::IsTriggerRightKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::D) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_RIGHT) ||
		(m_currentRightStick.x >= m_DEADZONE && m_previousRightStick.x < m_DEADZONE) ||
		(m_currentLeftStick.x >= m_DEADZONE && m_previousLeftStick.x < m_DEADZONE));
}

bool ButiEngine::InputManager::IsTriggerLeftKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::A) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_LEFT) ||
		(m_currentRightStick.x <= -m_DEADZONE && m_previousRightStick.x > -m_DEADZONE) ||
		(m_currentLeftStick.x <= -m_DEADZONE && m_previousLeftStick.x > -m_DEADZONE));
}

bool ButiEngine::InputManager::IsTriggerUpKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::W) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_UP) ||
		(m_currentRightStick.y >= m_DEADZONE && m_previousRightStick.y < m_DEADZONE) ||
		(m_currentLeftStick.y >= m_DEADZONE && m_previousLeftStick.y < m_DEADZONE));
}

bool ButiEngine::InputManager::IsTriggerDownKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::S) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_DOWN) ||
		(m_currentRightStick.y <= -m_DEADZONE && m_previousRightStick.y > -m_DEADZONE) ||
		(m_currentLeftStick.y <= -m_DEADZONE && m_previousLeftStick.y > -m_DEADZONE));
}

bool ButiEngine::InputManager::IsTriggerDecideKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::Space) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_A));
}

bool ButiEngine::InputManager::IsTriggerCancelKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::Esc) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_B));
}

bool ButiEngine::InputManager::IsZoomInKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::E) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_UP));
}

bool ButiEngine::InputManager::IsZoomOutKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::Q) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_DOWN));
}

bool ButiEngine::InputManager::IsCameraResetKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::R) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_STICK_RIGHT));
}

bool ButiEngine::InputManager::IsPushAnyGameKey()
{
	return (IsPushRightKey() ||
		IsPushLeftKey() ||
		IsPushUpKey() ||
		IsPushDownKey() ||
		IsPushGatherKey());
}

void ButiEngine::InputManager::VibrationStart(const float arg_vibPower)
{
	//GameDevice::GetInput()->Vibration_L(arg_vibPower);
	GameDevice::GetInput()->Vibration_R(arg_vibPower);
}

void ButiEngine::InputManager::VibrationStop()
{
	//GameDevice::GetInput()->Vibration_L(0);
	GameDevice::GetInput()->Vibration_R(0);
}

bool ButiEngine::InputManager::IsPushGatherKey()
{
	return GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_A);
}

bool ButiEngine::InputManager::IsTriggerGatherKey()
{
	return GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_A);
}

bool ButiEngine::InputManager::IsTriggerPauseKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::Esc) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_START));
}

bool ButiEngine::InputManager::IsTriggerBombKey()
{
	return GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_BUTTON_RIGHT);
}

ButiEngine::Vector2 ButiEngine::InputManager::GetRightStick()
{
	Vector2 output = m_currentRightStick;

	if (abs(output.x) < m_DEADZONE) { output.x = 0.0f; }
	if (abs(output.y) < m_DEADZONE) { output.y = 0.0f; }
	return output;
}

ButiEngine::Vector2 ButiEngine::InputManager::GetLeftStick()
{
	Vector2 output = m_currentLeftStick;

	if (abs(output.x) < m_DEADZONE) { output.x = 0.0f; }
	if (abs(output.y) < m_DEADZONE) { output.y = 0.0f; }
	return output;
}

float ButiEngine::InputManager::GetRightTrigger()
{
	float output = GameDevice::GetInput()->GetRightTrigger();

	if (output < m_DEADZONE) { output = 0.0f; }

	return output;
}

float ButiEngine::InputManager::GetLeftTrigger()
{
	float output = GameDevice::GetInput()->GetLeftTrigger();

	if (output < m_DEADZONE) { output = 0.0f; }

	return output;
}
