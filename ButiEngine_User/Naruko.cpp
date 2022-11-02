#include "stdafx_u.h"
#include "Naruko.h"
#include "InputManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

void ButiEngine::Naruko::OnUpdate()
{
	Move();
}

void ButiEngine::Naruko::OnSet()
{
}

void ButiEngine::Naruko::OnRemove()
{
}

void ButiEngine::Naruko::OnShowUI()
{
	GUI::DragInt("TrackerIndex", m_index, 1.0, 0, 16);
}

void ButiEngine::Naruko::Start()
{
	m_vwp_inputManager = GetManager().lock()->GetGameObject("InputManager").lock()->GetGameComponent<InputManager>();
	m_vwp_rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Naruko::Clone()
{
	auto clone = ButiEngine::ObjectFactory::Create<Naruko>();
	clone->m_index = m_index;
	return clone;
}

void ButiEngine::Naruko::Dead()
{
}

void ButiEngine::Naruko::Move()
{
	//if (GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize() > m_index)
	//{
	//	MoveByVRTracker();
	//}

	MoveByGamePad();
}

void ButiEngine::Naruko::MoveByGamePad()
{
	Vector2 direction = m_vwp_inputManager.lock()->GetLeftStick();
	direction.Normalize();

	Vector3 velocity = Vector3(direction.x, 0.0f, direction.y);
	float speed = 4.0f;

	//m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetPosition(position);
	m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetVelocity(velocity * speed * GameDevice::GetWorldSpeed());
}

void ButiEngine::Naruko::MoveByVRTracker()
{
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_index], deviceMatrix);
	Vector3 pos = deviceMatrix.GetPosition();
	pos *= 10.0f;
	pos.y = gameObject.lock()->transform->GetLocalScale().y * 0.5f;
	gameObject.lock()->transform->SetLocalPosition(pos);
	gameObject.lock()->transform->SetLocalRotation(deviceMatrix.GetRemovePosition());

	m_vwp_rigidBodyComponent.lock()->TransformApply();


	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::Enter))
	{
		GameDevice::GetVRTrackerInput().SetOrigin(m_index);
	}
}
