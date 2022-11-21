#include "stdafx_u.h"
#include "FriendFacePart.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "FriendHead.h"
#include "ButiBulletWrap/ButiBulletWrap/PhysicsManager.h"
#include "ButiBulletWrap/ButiBulletWrap/PhysicsWorld.h"
#include "PauseManager.h"
#include "StageManager.h"

void ButiEngine::FriendFacePart::OnUpdate()
{
	if (!CanUpdate())
	{
		return;
	}

	switch (m_state)
	{
	case ButiEngine::FacePartState::Move:
		if (!m_isCollisionHead)
		{
			Move();
		}
		break;
	case ButiEngine::FacePartState::Chase:
		OnChase();
		break;
	case ButiEngine::FacePartState::Stop:
		break;
	default:
		break;
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
			}
		}
	);

	gameObject.lock()->AddCollisionLeaveReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				//タグ判定
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("FriendHead"))
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
	m_vwp_stageManager = GetManager().lock()->GetGameObject("StageManager").lock()->GetGameComponent<StageManager>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	m_movePattern = MovePattern::Straight;

	m_isCollisionHead = false;
	SetMoveDirection();
	m_moveSpeed = ButiRandom::GetRandom(m_minMoveSpeed, m_maxMoveSpeed, 100);

	Vector3 velocity = m_moveDirection * m_moveSpeed * GameDevice::GetWorldSpeed();

	gameObject.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->SetVelocity(velocity * 100);

	m_state = FacePartState::Move;
	m_vwp_head = GetManager().lock()->GetGameObject("FriendHead");
	m_vlp_lockOnTimer = ObjectFactory::Create<RelativeTimer>(30);
	m_vlp_chaseTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_isRayCast = false;
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
	case ButiEngine::MovePattern::Stay:
		break;
	case ButiEngine::MovePattern::Straight:
		MoveStraight();
		break;
	case ButiEngine::MovePattern::Throw:
		break;
	default:
		break;
	}

	if (m_isRayCast)
	{
		ButiBullet::PhysicsRaycastResult rayRes;
		if (gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetPhysicsManager()->GetActivePhysicsWorld()->
			Raycast(gameObject.lock()->transform->GetWorldPosition(), -gameObject.lock()->transform->GetFront(), 100, 65531, &rayRes))
		{
			if (rayRes.physicsObject->GetOwnerData() == m_vwp_head)
			{
				//if (!m_vlp_lockOnTimer->IsOn())
				//{
				//	m_vlp_lockOnTimer->Start();
				//}

				//if (m_vlp_lockOnTimer->Update())
				//{
				//	m_vlp_lockOnTimer->Stop();

				//	if (!m_vwp_chaseTarget.lock())
				//	{
				//		m_state = FacePartState::Chase;
				//		m_vwp_chaseTarget = GetManager().lock()->AddObject(ObjectFactory::Create<Transform>(rayRes.point), gameObject.lock()->GetGameObjectName() + "ChaseTarget");
				//		m_vwp_chaseTarget.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform);
				//		gameObject.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->SetVelocity(Vector3Const::Zero);
				//		auto rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
				//		if (rigidBodyComponent)
				//		{
				//			rigidBodyComponent->SetIsRemove(true);
				//		}
				//		m_vlp_chaseTimer->Start();
				//	}
				//}

				auto headComponent = m_vwp_head.lock()->GetGameComponent<FriendHead>();
				if (headComponent)
				{
					if (headComponent->IsHighSpeed())
					{
						m_state = FacePartState::Chase;
						Vector3 headVelocity = headComponent->GetVelocity();
						m_vwp_chaseTarget = GetManager().lock()->AddObject(ObjectFactory::Create<Transform>(rayRes.point + headVelocity), gameObject.lock()->GetGameObjectName() + "ChaseTarget");
						m_vwp_chaseTarget.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform);
						gameObject.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->SetVelocity(Vector3Const::Zero);
						auto rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
						if (rigidBodyComponent)
						{
							rigidBodyComponent->SetIsRemove(true);
						}
						m_vlp_chaseTimer->Start();
					}
				}
			}
		}
		else
		{
			m_vlp_lockOnTimer->Reset();
			m_vlp_lockOnTimer->Stop();
		}
	}
}

void ButiEngine::FriendFacePart::MoveStraight()
{
	Vector3 velocity = m_moveDirection * m_moveSpeed * GameDevice::GetWorldSpeed();
	/*gameObject.lock()->transform->Translate(velocity);
	gameObject.lock()->GetGameComponent<RigidBodyComponent>()->TransformApply();*/

	gameObject.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->SetVelocity(velocity * 100);
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

void ButiEngine::FriendFacePart::OnChase()
{
	if (m_vlp_chaseTimer->IsOn())
	{
		Vector3 pos = MathHelper::LerpPosition(gameObject.lock()->transform->GetLocalPosition(), m_vwp_chaseTarget.lock()->transform->GetWorldPosition(), m_vlp_chaseTimer->GetPercent());
		gameObject.lock()->transform->SetLocalPosition(pos);
	}

	if (m_vlp_chaseTimer->Update())
	{
		m_vlp_chaseTimer->Stop();

		gameObject.lock()->transform->SetLocalPosition(m_vwp_chaseTarget.lock()->transform->GetWorldPosition());
		gameObject.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform);

		m_vwp_chaseTarget.lock()->SetIsRemove(true);
		m_vwp_chaseTarget = Value_weak_ptr<GameObject>();


		m_state = FacePartState::Stop;
	}
}

void ButiEngine::FriendFacePart::OnCollisionFriendHead(Value_weak_ptr<GameObject> arg_vwp_gameObject)
{
	if (m_isRayCast)
	{
		return;
	}

	m_isCollisionHead = true;
	gameObject.lock()->transform->SetBaseTransform(arg_vwp_gameObject.lock()->transform);

	auto rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
	if (rigidBodyComponent)
	{
		rigidBodyComponent->SetIsRemove(true);
	}
}

bool ButiEngine::FriendFacePart::CanUpdate()
{
	if(!m_vwp_stageManager.lock()->IsGameStart() && m_)

	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return false;
	}

	return true;
}
