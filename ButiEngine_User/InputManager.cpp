#include "stdafx_u.h"
#include "InputManager.h"
#include<Xinput.h>

constexpr float DEADZONE = 0.25f;

void ButiEngine::InputManager::OnUpdate()
{
	m_previousRightStick = m_currentRightStick;
	m_previousLeftStick = m_currentLeftStick;

	m_currentRightStick = GameDevice::GetInput().GetRightStick();
	m_currentLeftStick = GameDevice::GetInput().GetLeftStick();
}

void ButiEngine::InputManager::OnSet()
{
}

void ButiEngine::InputManager::Start()
{
	m_isNotPushKey = true;
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
	return (GameDevice::GetInput().CheckKey(ButiInput::Keys::D) ||
		GameDevice::GetInput().GetPadButton(ButiInput::PadButtons::XBOX_RIGHT) ||
		m_currentRightStick.x >= DEADZONE ||
		m_currentLeftStick.x >= DEADZONE);
}

bool ButiEngine::InputManager::IsPushLeftKey()
{
	return (GameDevice::GetInput().CheckKey(ButiInput::Keys::A) ||
		GameDevice::GetInput().GetPadButton(ButiInput::PadButtons::XBOX_LEFT) ||
		m_currentRightStick.x <= -DEADZONE ||
		m_currentLeftStick.x <= -DEADZONE);
}

bool ButiEngine::InputManager::IsPushUpKey()
{
	return (GameDevice::GetInput().CheckKey(ButiInput::Keys::W) ||
		GameDevice::GetInput().GetPadButton(ButiInput::PadButtons::XBOX_UP) ||
		m_currentRightStick.y >= DEADZONE ||
		m_currentLeftStick.y >= DEADZONE);
}

bool ButiEngine::InputManager::IsPushDownKey()
{
	return (GameDevice::GetInput().CheckKey(ButiInput::Keys::S) ||
		GameDevice::GetInput().GetPadButton(ButiInput::PadButtons::XBOX_DOWN) ||
		m_currentRightStick.y <= -DEADZONE ||
		m_currentLeftStick.y <= -DEADZONE);
}

bool ButiEngine::InputManager::IsTriggerRightKey()
{
	return (GameDevice::GetInput().TriggerKey(ButiInput::Keys::D) ||
		GameDevice::GetInput().GetPadButtonTrigger(ButiInput::PadButtons::XBOX_RIGHT) ||
		(m_currentRightStick.x >= DEADZONE && m_previousRightStick.x < DEADZONE) ||
		(m_currentLeftStick.x >= DEADZONE && m_previousLeftStick.x < DEADZONE));
}

bool ButiEngine::InputManager::IsTriggerLeftKey()
{
	return (GameDevice::GetInput().TriggerKey(ButiInput::Keys::A) ||
		GameDevice::GetInput().GetPadButtonTrigger(ButiInput::PadButtons::XBOX_LEFT) ||
		(m_currentRightStick.x <= -DEADZONE && m_previousRightStick.x > -DEADZONE) ||
		(m_currentLeftStick.x <= -DEADZONE && m_previousLeftStick.x > -DEADZONE));
}

bool ButiEngine::InputManager::IsTriggerUpKey()
{
	return (GameDevice::GetInput().TriggerKey(ButiInput::Keys::W) ||
		GameDevice::GetInput().GetPadButtonTrigger(ButiInput::PadButtons::XBOX_UP) ||
		(m_currentRightStick.y >= DEADZONE && m_previousRightStick.y < DEADZONE) ||
		(m_currentLeftStick.y >= DEADZONE && m_previousLeftStick.y < DEADZONE));
}

bool ButiEngine::InputManager::IsTriggerDownKey()
{
	return (GameDevice::GetInput().TriggerKey(ButiInput::Keys::S) ||
		GameDevice::GetInput().GetPadButtonTrigger(ButiInput::PadButtons::XBOX_DOWN) ||
		(m_currentRightStick.y <= -DEADZONE && m_previousRightStick.y > -DEADZONE) ||
		(m_currentLeftStick.y <= -DEADZONE && m_previousLeftStick.y > -DEADZONE));
}

bool ButiEngine::InputManager::IsTriggerDecideKey()
{
	return (//GameDevice::GetInput().TriggerKey(ButiInput::Keys::Space) ||
		GameDevice::GetInput().GetPadButtonTrigger(ButiInput::PadButtons::XBOX_A));
}

bool ButiEngine::InputManager::IsTriggerCancelKey()
{
	return (GameDevice::GetInput().TriggerKey(ButiInput::Keys::Esc) ||
		GameDevice::GetInput().GetPadButtonTrigger(ButiInput::PadButtons::XBOX_B));
}

bool ButiEngine::InputManager::IsPushAnyGameKey()
{
	return (IsPushRightKey() ||
		IsPushLeftKey() ||
		IsPushUpKey() ||
		IsPushDownKey());
}

ButiEngine::Vector2 ButiEngine::InputManager::GetMousePosition()
{
	return GameDevice::GetInput().GetMousePos();
}

void ButiEngine::InputManager::SetRightVibrationPower(const float arg_power)
{
	GameDevice::GetInput().Vibration_R(arg_power);
}

void ButiEngine::InputManager::SetLeftVibrationPower(const float arg_power)
{
	GameDevice::GetInput().Vibration_L(arg_power);
}

ButiEngine::Vector2 ButiEngine::InputManager::GetRightStick()
{
	Vector2 output = m_currentRightStick;

	if (abs(output.x) < DEADZONE) { output.x = 0.0f; }
	if (abs(output.y) < DEADZONE) { output.y = 0.0f; }
	return output;
}

ButiEngine::Vector2 ButiEngine::InputManager::GetLeftStick()
{
	Vector2 output = m_currentLeftStick;

	if (abs(output.x) < DEADZONE) { output.x = 0.0f; }
	if (abs(output.y) < DEADZONE) { output.y = 0.0f; }
	return output;
}

float ButiEngine::InputManager::GetRightTrigger()
{
	float output = GameDevice::GetInput().GetRightTrigger();

	if (output < DEADZONE) { output = 0.0f; }

	return output;
}

float ButiEngine::InputManager::GetLeftTrigger()
{
	float output = GameDevice::GetInput().GetLeftTrigger();

	if (output < DEADZONE) { output = 0.0f; }

	return output;
}
