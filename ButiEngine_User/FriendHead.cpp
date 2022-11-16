#include "stdafx_u.h"
#include "FriendHead.h"
#include "InputManager.h"
#include "GameSettings.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

void ButiEngine::FriendHead::OnUpdate()
{
	Control();
	CalcVelocity();
	if (GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize() > m_trackerIndex)
	{
		CheckPut();
	}

	if (GameDevice::GetInput().CheckKey(ButiInput::Keys::B))
	{
		auto box = GetManager().lock()->AddObjectFromCereal("Box");

	}
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

	m_vwp_rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();

	m_prevPos = Vector3Const::Zero;
	m_crntPos = Vector3Const::Zero;
	m_velocity = Vector3Const::Zero;

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
		gameObject.lock()->transform->RollLocalRotationZ_Degrees(-2.0f);
	}
	else if (GameDevice::GetInput().GetPadButton(ButiInput::PadButtons::XBOX_BUTTON_LEFT))
	{
		gameObject.lock()->transform->RollLocalRotationZ_Degrees(2.0f);
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
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	std::string sceneName = sceneManager->GetCurrentScene()->GetSceneInformation()->GetSceneName();
	sceneManager->RemoveScene(sceneName);
	sceneManager->LoadScene(sceneName);
	sceneManager->ChangeScene(sceneName);
}

void ButiEngine::FriendHead::CalcVelocity()
{
	m_prevPos = m_crntPos;
	m_crntPos = gameObject.lock()->transform->GetLocalPosition();

	m_velocity = m_crntPos - m_prevPos;

	constexpr float fastBorder = 0.1f;

	if (m_velocity.z >= fastBorder)
	{
		m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetCollisionGroup(2);
	}
	else
	{
		m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetCollisionGroup(1);
	}
}

void ButiEngine::FriendHead::CheckPut()
{
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
