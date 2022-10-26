#include "stdafx_u.h"
#include "Soldier.h"
#include "SeparateDrawObject.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

constexpr float MAX_MOVE_SPEED = 3.5f;

void ButiEngine::Soldier::OnUpdate()
{
	switch (m_state)
	{
	case ButiEngine::SoldierState::Sleep:
		Sleep();
		break;
	case ButiEngine::SoldierState::Active:
		break;
	case ButiEngine::SoldierState::Home:
		break;
	default:
		break;
	}
	Move();
}

void ButiEngine::Soldier::OnSet()
{
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				//タグ判定
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("Home"))
				{
					m_state = SoldierState::Home;
					m_vwp_drawObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color = ButiColor::White();
				}
			}
		}
	);
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
	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	m_vwp_naruko = GetManager().lock()->GetGameObject("Naruko");
	m_vwp_rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();

	m_state = SoldierState::Sleep;
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
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
}

void ButiEngine::Soldier::Sleep()
{
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 narukoPos = m_vwp_naruko.lock()->transform->GetLocalPosition();

	//鳴子との距離が検知範囲より近かったら覚醒状態にする
	float distanceSqr = (pos - narukoPos).GetLengthSqr();
	float rangeSqr = m_detectionRange * m_detectionRange;
	if (distanceSqr <= rangeSqr)
	{
		m_state = SoldierState::Active;
		m_vwp_drawObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color = ButiColor::LightGreen();
	}
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
	float targetSpeed = 0.0f;
	float easingSpeed = 0.01f;
	switch (m_state)
	{
	case ButiEngine::SoldierState::Sleep:
		break;
	case ButiEngine::SoldierState::Active:
		targetSpeed = MAX_MOVE_SPEED;
		break;
	case ButiEngine::SoldierState::Home:
		easingSpeed = 0.05f;
		break;
	default:
		break;
	}

	m_moveSpeed = MathHelper::Lerp(m_moveSpeed, targetSpeed, easingSpeed);
}
