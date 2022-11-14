#include "stdafx_u.h"
#include "FriendFacePart.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Header/GameObjects/DefaultGameComponent/TriggerComponent.h"
#include "FriendHead.h"

void ButiEngine::FriendFacePart::OnUpdate()
{
	if (m_canMove)
	{
		Move();
	}
}

void ButiEngine::FriendFacePart::OnSet()
{
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				//タグ判定
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("DeadArea"))
				{
					gameObject.lock()->SetIsRemove(true);
				}
				else if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("FriendHead"))
				{
					OnCollisionFriendHead(arg_other.vwp_gameObject);
				}
			}
		}
	);
}

void ButiEngine::FriendFacePart::OnRemove()
{
}

void ButiEngine::FriendFacePart::OnShowUI()
{
	GUI::BulletText(U8("最低速度"));
	if (GUI::DragFloat("##MinMoveSpeed", &m_minMoveSpeed, 0.01f, 0.0f, 100.0f))
	{
		m_moveSpeed = ButiRandom::GetRandom(m_minMoveSpeed, m_maxMoveSpeed, 100);
	}

	GUI::BulletText(U8("最高速度"));
	if (GUI::DragFloat("##MaxMoveSpeed", &m_maxMoveSpeed, 0.01f, 0.0f, 100.0f))
	{
		m_moveSpeed = ButiRandom::GetRandom(m_minMoveSpeed, m_maxMoveSpeed, 100);
	}
}

void ButiEngine::FriendFacePart::Start()
{
	m_movePattern = MovePattern::Straight;

	m_canMove = true;
	SetMoveDirection();
	m_moveSpeed = ButiRandom::GetRandom(m_minMoveSpeed, m_maxMoveSpeed, 100);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendFacePart::Clone()
{
	auto clone = ObjectFactory::Create<FriendFacePart>();
	clone->m_minMoveSpeed = m_minMoveSpeed;
	clone->m_maxMoveSpeed = m_maxMoveSpeed;
	return clone;
}

void ButiEngine::FriendFacePart::Move()
{
	switch (m_movePattern)
	{
	case ButiEngine::MovePattern::Straight:
		MoveStraight();
		break;
	default:
		break;
	}
}

void ButiEngine::FriendFacePart::MoveStraight()
{
	Vector3 velocity = m_moveDirection * m_moveSpeed * GameDevice::GetWorldSpeed();
	gameObject.lock()->transform->Translate(velocity);
}

void ButiEngine::FriendFacePart::SetMoveDirection()
{
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 centerPos = Vector3Const::Zero;
	centerPos.z = pos.z;

	Vector3 diff = centerPos - pos;
	m_moveDirection = diff * Matrix4x4::RollZ(MathHelper::ToRadian(ButiRandom::GetRandom(-20.0f, 20.0f, 10)));

	m_moveDirection.Normalize();
}

void ButiEngine::FriendFacePart::OnCollisionFriendHead(Value_weak_ptr<GameObject> arg_vwp_gameObject)
{
	if (arg_vwp_gameObject.lock()->GetGameComponent<FriendHead>()->GetVelocity().z >= 0.1f)
	{
		m_canMove = false;
		gameObject.lock()->transform->SetBaseTransform(arg_vwp_gameObject.lock()->transform);

		auto triggetComponent = gameObject.lock()->GetGameComponent<TriggerComponent>();
		if (triggetComponent)
		{
			triggetComponent->SetIsRemove(true);
		}
	}
}
