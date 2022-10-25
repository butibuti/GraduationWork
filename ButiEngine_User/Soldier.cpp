#include "stdafx_u.h"
#include "Soldier.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

constexpr float MAX_MOVE_SPEED = 3.5f;

void ButiEngine::Soldier::OnUpdate()
{
	Move();
}

void ButiEngine::Soldier::OnSet()
{
}

void ButiEngine::Soldier::OnRemove()
{
}

void ButiEngine::Soldier::OnShowUI()
{
	GUI::BulletText("DetectionRange");
	GUI::DragFloat("##detectionRange", m_detectionRange, 1.0, 0.0f, 100.0f);
}

void ButiEngine::Soldier::Start()
{
	m_vwp_naruko = GetManager().lock()->GetGameObject("Naruko");
	m_vwp_rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();

	m_state = SoldierState::Active;
	m_moveSpeed = 0.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Soldier::Clone()
{
	auto clone = ButiEngine::ObjectFactory::Create<Soldier>();
	clone->m_detectionRange = m_detectionRange;
	return clone;
}

void ButiEngine::Soldier::Dead()
{
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::Soldier::Move()
{
	Vector2 moveDirection;
	SetMoveDirection(moveDirection);
	SetMoveSpeed();

	Vector3 velocity = Vector3(moveDirection.x, 0.0f, moveDirection.y) * m_moveSpeed;
	m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetVelocity(velocity * GameDevice::GetWorldSpeed());
}

void ButiEngine::Soldier::SetMoveDirection(Vector2& arg_ref_moveDirection)
{
	Vector3 narukoPos = m_vwp_naruko.lock()->transform->GetLocalPosition();
	Vector3 dir = narukoPos - gameObject.lock()->transform->GetLocalPosition();
	dir.Normalize();

	arg_ref_moveDirection.x = dir.x;
	arg_ref_moveDirection.y = dir.z;
}

void ButiEngine::Soldier::SetMoveSpeed()
{
	float targetSpeed;
	switch (m_state)
	{
	case ButiEngine::SoldierState::Sleep:
		targetSpeed = 0.0f;
		break;
	case ButiEngine::SoldierState::Active:
		targetSpeed = MAX_MOVE_SPEED;
		break;
	default:
		break;
	}

	constexpr float easingSpeed = 0.1f;
	m_moveSpeed = MathHelper::Lerp(m_moveSpeed, targetSpeed, easingSpeed);
}
