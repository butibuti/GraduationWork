#include "stdafx_u.h"
#include "FriendHead.h"
#include "InputManager.h"

void ButiEngine::FriendHead::OnUpdate()
{
	Move();
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
}

void ButiEngine::FriendHead::Start()
{
	m_vwp_inputManager = GetManager().lock()->GetGameObject("InputManager").lock()->GetGameComponent<InputManager>();
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

void ButiEngine::FriendHead::Move()
{
	if (GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize() > m_trackerIndex)
	{
		MoveByVRTracker();
	}

	MoveByGamePad();

	Vector3 moveLimit = Vector3(6.0f, 3.5f, 1.5f);

	Vector3 position = gameObject.lock()->transform->GetLocalPosition();

	position.x = min(position.x, moveLimit.x);
	position.x = max(position.x, -moveLimit.x);
	position.y = min(position.y, moveLimit.y);
	position.y = max(position.y, -moveLimit.y);
	position.z = min(position.z, moveLimit.z);
	position.z = max(position.z, -moveLimit.z);

	gameObject.lock()->transform->SetLocalPosition(position);
}

void ButiEngine::FriendHead::MoveByGamePad()
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

	direction.Normalize();

	constexpr float moveSpeed = 0.1f;

	Vector3 velocity = direction * moveSpeed * GameDevice::GetWorldSpeed();
	gameObject.lock()->transform->Translate(velocity);
}

void ButiEngine::FriendHead::MoveByVRTracker()
{
}
