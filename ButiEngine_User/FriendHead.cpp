#include "stdafx_u.h"
#include "FriendHead.h"
#include "InputManager.h"
#include "GameSettings.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "PauseManager.h"
#include "FriendManager.h"

void ButiEngine::FriendHead::OnUpdate()
{
	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return;
	}

	Control();
	CalcVelocity();
	CheckPut();

#ifdef DEBUG
	if (GameDevice::GetInput().CheckKey(ButiInput::Keys::B))
	{
		GetManager().lock()->AddObjectFromCereal("Box");
	}
#endif // DEBUG
}

void ButiEngine::FriendHead::OnSet()
{
}

void ButiEngine::FriendHead::OnRemove()
{
}

void ButiEngine::FriendHead::OnShowUI()
{
	GUI::DragInt("TrackerIndex", m_trackerIndex, 1.0, 0, 16);

	GUI::BulletText("Velocity");
	GUI::Text("x:" + std::to_string(m_velocity.x) + " y:" + std::to_string(m_velocity.y) + " z:" + std::to_string(m_velocity .z));

	GUI::Text("Speed:" + std::to_string(m_velocity.GetLength()));
}

void ButiEngine::FriendHead::Start()
{
	m_vwp_inputManager = GetManager().lock()->GetGameObject("InputManager").lock()->GetGameComponent<InputManager>();
	m_vwp_gameSettings = GetManager().lock()->GetGameObject("GameSettings").lock()->GetGameComponent<GameSettings>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_friendManager = GetManager().lock()->GetGameObject("FriendManager").lock()->GetGameComponent<FriendManager>();

	m_vwp_rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();

	m_prevPos = Vector3Const::Zero;
	m_crntPos = Vector3Const::Zero;
	m_velocity = Vector3Const::Zero;

	m_eyeCount = 0;
	m_noseCount = 0;
	m_mouthCount = 0;

	m_maxEyeCount = 2;
	m_maxNoseCount = 1;
	m_maxMouthCount = 1;

	m_vlp_putTimer = ObjectFactory::Create<RelativeTimer>(60);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendHead::Clone()
{
	auto clone = ObjectFactory::Create<FriendHead>();
	clone->m_trackerIndex = m_trackerIndex;
	return clone;
}

void ButiEngine::FriendHead::Dead()
{
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::FriendHead::Control()
{
	if (GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize() > m_trackerIndex)
	{
		ControlByVRTracker();
	}

	ControlByGamePad();

	m_vwp_rigidBodyComponent.lock()->TransformApply();

	//Vector3 moveLimit = m_vwp_gameSettings.lock()->GetHeadMoveLimit();

	//Vector3 position = gameObject.lock()->transform->GetLocalPosition();

	//position.x = min(position.x, moveLimit.x);
	//position.x = max(position.x, -moveLimit.x);
	//position.y = min(position.y, moveLimit.y);
	//position.y = max(position.y, -moveLimit.y);
	//position.z = min(position.z, moveLimit.z);
	//position.z = max(position.z, -moveLimit.z);

	//gameObject.lock()->transform->SetLocalPosition(position);
}

void ButiEngine::FriendHead::ControlByGamePad()
{
	Vector3 direction = Vector3Const::Zero;

	Vector2 leftStick = m_vwp_inputManager.lock()->GetLeftStick();
	if (leftStick.GetLengthSqr() != 0.0f)
	{
		direction = leftStick;
	}

	Vector2 rightStick = m_vwp_inputManager.lock()->GetRightStick();
	if (rightStick.GetLength() != 0.0f)
	{
		direction.z = rightStick.y * 0.5f;
	}

	direction.x *= -1.0f;
	direction.z *= -1.0f;
	direction.Normalize();

	constexpr float moveSpeed = 0.1f;

	Vector3 velocity = direction * moveSpeed * GameDevice::GetWorldSpeed();
	gameObject.lock()->transform->Translate(velocity);

	if (GameDevice::GetInput().GetPadButton(ButiInput::PadButtons::XBOX_BUTTON_RIGHT))
	{
		gameObject.lock()->transform->RollLocalRotationY_Degrees(-2.0f);
	}
	else if (GameDevice::GetInput().GetPadButton(ButiInput::PadButtons::XBOX_BUTTON_LEFT))
	{
		gameObject.lock()->transform->RollLocalRotationY_Degrees(2.0f);
	}
}

void ButiEngine::FriendHead::ControlByVRTracker()
{
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_trackerIndex], deviceMatrix);
	Vector3 pos = deviceMatrix.GetPosition();
	pos *= m_vwp_gameSettings.lock()->GetCorrection();
	pos.x *= -1; 
	auto rotation = deviceMatrix.GetRemovePosition();
	rotation._13 *= -1;
	rotation._31 *= -1;
	rotation._12 *= -1;
	rotation._21 *= -1;
	gameObject.lock()->transform->SetLocalPosition(pos);
	gameObject.lock()->transform->SetLocalRotation(rotation);
}

void ButiEngine::FriendHead::OnPut()
{
	m_vwp_friendManager.lock()->AddFriendCount();
	
	SetIsRemove(true);
	m_vwp_rigidBodyComponent.lock()->SetIsRemove(true);
	gameObject.lock()->transform->TranslateX(ButiRandom::GetInt(-5, 5));
	gameObject.lock()->transform->TranslateY(ButiRandom::GetInt(-2, 2));
	gameObject.lock()->transform->TranslateZ(ButiRandom::GetInt(-10, -5));

	auto newHead = GetManager().lock()->AddObjectFromCereal("FriendHead");

	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_trackerIndex], deviceMatrix);
	Vector3 pos = deviceMatrix.GetPosition();
	pos *= m_vwp_gameSettings.lock()->GetCorrection();
	pos.x *= -1;
	auto rotation = deviceMatrix.GetRemovePosition();
	rotation._13 *= -1;
	rotation._31 *= -1;
	rotation._12 *= -1;
	rotation._21 *= -1;
	newHead.lock()->transform->SetLocalPosition(pos);
	newHead.lock()->transform->SetLocalRotation(rotation);
}

void ButiEngine::FriendHead::CalcVelocity()
{
	m_prevPos = m_crntPos;
	m_crntPos = gameObject.lock()->transform->GetLocalPosition();

	m_velocity = m_crntPos - m_prevPos;

	if (IsHighSpeed())
	{
		m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetCollisionGroup(2);
	}
	else
	{
		m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetCollisionGroup(2);
	}
}

void ButiEngine::FriendHead::CheckPut()
{
	if (!CanPut())
	{
		return;
	}

	if (GameDevice::GetInput().GetPadButtonTrigger(ButiInput::PadButtons::XBOX_A))
	{
		OnPut();
	}

	if (GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize() <= m_trackerIndex)
	{
		return;
	}

	//‘ä‚É‹ß‚­‚ÄˆÚ“®‘¬“x‚ª’x‚©‚Á‚½‚ç’u‚¢‚½‚Æ”»’è‚·‚é
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_trackerIndex], deviceMatrix);
	Vector3 pos = deviceMatrix.GetPosition();
	Vector3 tablePos = m_vwp_gameSettings.lock()->GetTablePos();

	constexpr float putTelerance = 0.1f;
	constexpr float putMoveSpeedBorder = 0.01f;

	float distanceSqr = (pos - tablePos).GetLengthSqr();
	float putTleranceSqr = putTelerance * putTelerance;

	float moveSpeedSqr = abs(m_velocity.GetLengthSqr());
	float putMoveSpeedBorderSqr = putMoveSpeedBorder * putMoveSpeedBorder;

	if (distanceSqr <= putTleranceSqr && moveSpeedSqr <= putMoveSpeedBorderSqr)
	{
		if (!m_vlp_putTimer->IsOn())
		{
			m_vlp_putTimer->Start();
		}

		if (m_vlp_putTimer->Update())
		{
			OnPut();
		}
	}
	else
	{
		m_vlp_putTimer->Reset();
	}
}

bool ButiEngine::FriendHead::CanPut()
{
	constexpr std::int32_t necessaryEyeCount = 2;
	constexpr std::int32_t necessaryNoseCount = 1;
	constexpr std::int32_t necessaryMouthCount = 1;

	if (m_eyeCount < necessaryEyeCount)
	{
		return false;
	}

	if (m_noseCount < necessaryNoseCount)
	{
		return false;
	}

	if (m_mouthCount < necessaryMouthCount)
	{
		return false;
	}

	return true;
}
