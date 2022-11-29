#include "stdafx_u.h"
#include "FriendFacePart.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "FriendHead.h"
#include "ButiBulletWrap/ButiBulletWrap/PhysicsManager.h"
#include "ButiBulletWrap/ButiBulletWrap/PhysicsWorld.h"
#include "PauseManager.h"
#include "StageManager.h"
#include "SeparateDrawObject.h"
#include "PartStickAnimation.h"
#include "SeparateDrawObject.h"
#include "FriendHead_PartHitArea.h"

void ButiEngine::FriendFacePart::OnUpdate()
{
	if (m_vlp_changeGroupMaskTimer->Update())
	{
		m_vlp_changeGroupMaskTimer->Stop();
		ChangeGroupMask();
		
	}

	if (!CanUpdate())
	{
		if (m_vwp_pauseManager.lock()->IsPause())
		{
			if (m_vwp_rigidBodyComponent.lock())
			{
				m_vwp_rigidBodyComponent.lock()->SetIsAffectedGravity(false);
				m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetVelocity(Vector3Const::Zero);
			}
		}
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
		Chase();
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
					gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->SetIsRemove(true);
				}
				else if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("PartHitArea"))
				{
					OnCollisionPartHitArea(arg_other.vwp_gameObject);
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
					m_isCollisionHead = true;
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
	std::string typeStr;
	switch (m_type)
	{
	case ButiEngine::PartType::Eye:
		typeStr = "Eye";
		break;
	case ButiEngine::PartType::Nose:
		typeStr = "Nose";
		break;
	case ButiEngine::PartType::Mouth:
		typeStr = "Mouth";
		break;
	case ButiEngine::PartType::Dummy:
		typeStr = "Dummy";
		break;
	default:
		break;
	}

	GUI::BulletText("PartType:" + typeStr);

	if (GUI::Button("Eye"))
	{
		m_type = PartType::Eye;
	}
	GUI::SameLine();
	if (GUI::Button("Nose"))
	{
		m_type = PartType::Nose;
	}
	GUI::SameLine();
	if (GUI::Button("Mouth"))
	{
		m_type = PartType::Mouth;
	}
	GUI::SameLine();
	if (GUI::Button("Dummy"))
	{
		m_type = PartType::Dummy;
	}

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

	m_vwp_rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();

	m_vlp_changeGroupMaskTimer = ObjectFactory::Create<RelativeTimer>(30);
	m_vlp_changeGroupMaskTimer->Start();

	m_isCollisionHead = false;
	SetMoveDirection();
	m_moveSpeed = ButiRandom::GetRandom(m_minMoveSpeed, m_maxMoveSpeed, 100);

	if (m_movePattern == MovePattern::Throw)
	{
		m_vwp_rigidBodyComponent.lock()->SetIsAffectedGravity(true);
		m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetGravity(Vector3(0.0f, -1.5f, 0.0f));
		Vector3 velocity = m_moveDirection * m_moveSpeed * GameDevice::GetWorldSpeed();
		m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetVelocity(velocity * 500);
	}

	m_state = FacePartState::Move;
	m_vwp_head = GetManager().lock()->GetGameObject("FriendHead");
	m_vlp_lockOnTimer = ObjectFactory::Create<RelativeTimer>(30);
	m_vlp_chaseTimer = ObjectFactory::Create<RelativeTimer>(30);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendFacePart::Clone()
{
	auto clone = ObjectFactory::Create<FriendFacePart>();
	clone->m_minMoveSpeed = m_minMoveSpeed;
	clone->m_maxMoveSpeed = m_maxMoveSpeed;
	clone->m_type = m_type;
	return clone;
}

void ButiEngine::FriendFacePart::Move()
{
	switch (m_movePattern)
	{
	case ButiEngine::MovePattern::Stay:
		MoveStay();
		break;
	case ButiEngine::MovePattern::Straight:
		MoveStraight();
		break;
	case ButiEngine::MovePattern::Throw:
		MoveThrow();
		break;
	default:
		break;
	}
}

void ButiEngine::FriendFacePart::MoveStay()
{
	//m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetVelocity(Vector3Const::Zero);
}

void ButiEngine::FriendFacePart::MoveStraight()
{
	Vector3 velocity = m_moveDirection * m_moveSpeed * GameDevice::GetWorldSpeed();
	m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetVelocity(velocity * 100);
}

void ButiEngine::FriendFacePart::MoveThrow()
{
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

void ButiEngine::FriendFacePart::StickToFriendHead()
{
	auto head = GetManager().lock()->GetGameObject(GameObjectTag("FriendHead"));
	gameObject.lock()->transform->SetBaseTransform(head.lock()->transform);

	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("FriendFacePart"));

	m_vwp_rigidBodyComponent.lock()->SetIsRemove(true);
	gameObject.lock()->GetGameComponent<MeshDrawComponent>()->SetIsRemove(true);

	auto drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	drawObject.lock()->AddGameComponent<PartStickAnimation>();
}

void ButiEngine::FriendFacePart::Chase()
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

void ButiEngine::FriendFacePart::ChangeGroupMask()
{
	std::int32_t mask;
	switch (m_type)
	{
	case ButiEngine::PartType::Eye:
		mask = 65329;
		break;
	case ButiEngine::PartType::Nose:
		mask = 65361;
		break;
	case ButiEngine::PartType::Mouth:
		mask = 65425;
		break;
	case ButiEngine::PartType::Dummy:
		break;
	default:
		break;
	}

	m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetCollisionGroupMask(mask);
}

void ButiEngine::FriendFacePart::OnCollisionPartHitArea(Value_weak_ptr<GameObject> arg_vwp_partHitArea)
{
	auto partHitAreaComponent = arg_vwp_partHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();

	if (partHitAreaComponent->CanStickPart(m_type))
	{
		StickToFriendHead();
		partHitAreaComponent->StickPart(gameObject, m_type);
	}
}

bool ButiEngine::FriendFacePart::CanUpdate()
{
	if (!m_vwp_stageManager.lock()->IsGameStart() && m_movePattern != MovePattern::Stay)
	{
		return false;
	}

	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return false;
	}

	return true;
}
