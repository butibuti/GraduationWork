#include "stdafx_u.h"
#include "Enemy_Test.h"
#include "Soldier.h"
#include "SeparateDrawObject.h"
#include "SoldierManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include"ButiBulletWrap/ButiBulletWrap/PhysicsManager.h"
#include"ButiBulletWrap/ButiBulletWrap/PhysicsWorld.h"
#include"ButiBulletWrap/ButiBulletWrap/Joint.h"

void ButiEngine::Enemy_Test::OnUpdate()
{
	SetTargetSoldier();
	Move();
	OnReturn();
}

void ButiEngine::Enemy_Test::OnSet()
{
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				//タグ判定
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("Soldier"))
				{
					auto soldierComponent = arg_other.vwp_gameObject.lock()->GetGameComponent<Soldier>();
					auto soldierState = soldierComponent->GetState();
					switch (soldierState)
					{
					case ButiEngine::SoldierState::Sleep:
						break;
					case ButiEngine::SoldierState::Active:
						if (m_vwp_abductionSoldier.lock())
						{
							m_vwp_abductionSoldier.lock()->GetGameComponent<Soldier>()->Sleep();
						}
						Dead();
						break;
					case ButiEngine::SoldierState::Home:
						if (!m_vwp_abductionSoldier.lock())
						{
							m_state = EnemyState::Return;
							m_vwp_abductionSoldier = arg_other.vwp_gameObject;
							soldierComponent->Abduction(gameObject);

							CreateJoint();
						}
						break;
					default:
						break;
					}
				}
			}
		}
	);
}

void ButiEngine::Enemy_Test::OnRemove()
{
}

void ButiEngine::Enemy_Test::OnShowUI()
{
}

void ButiEngine::Enemy_Test::Start()
{
	m_vwp_soldierManager = GetManager().lock()->GetGameObject("SoldierManager").lock()->GetGameComponent<SoldierManager>();
	SetTargetSoldier();
	m_vwp_rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();

	m_state = EnemyState::Chase;
	m_spawnPos = gameObject.lock()->transform->GetLocalPosition();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Test::Clone()
{
	return ObjectFactory::Create<Enemy_Test>();
}

void ButiEngine::Enemy_Test::Dead()
{
	DestroyJoint();
	gameObject.lock()->SetIsRemove(true);
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
}

void ButiEngine::Enemy_Test::Move()
{
	Vector3 targetPos;
	switch (m_state)
	{
	case ButiEngine::EnemyState::Chase:
		targetPos = m_vwp_targetSoldier.lock()->transform->GetLocalPosition();
		break;
	case ButiEngine::EnemyState::Return:
		targetPos = m_spawnPos;
		break;
	default:
		break;
	}

	Vector3 dir = targetPos - gameObject.lock()->transform->GetLocalPosition();
	dir.y = 0.0f;
	dir.Normalize();

	constexpr float moveSpeed = 3.0f;
	Vector3 velocity = dir * moveSpeed;
	m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetVelocity(velocity * GameDevice::GetWorldSpeed());
}

void ButiEngine::Enemy_Test::SetTargetSoldier()
{
	if (m_state != EnemyState::Chase)
	{
		return;
	}

	if (m_vwp_soldierManager.lock()->IsInHome(m_vwp_targetSoldier))
	{
		return;
	}

	m_vwp_targetSoldier = m_vwp_soldierManager.lock()->GetHomeSoldierRandom();
	//ホームに兵士がいなかったら帰る
	if (!m_vwp_targetSoldier.lock())
	{
		m_state = EnemyState::Return;
	}
}

void ButiEngine::Enemy_Test::OnReturn()
{
	if (m_state != EnemyState::Return)
	{
		return;
	}

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	float distanceSqr = (pos - m_spawnPos).GetLengthSqr();
	if (distanceSqr < 10.0f)
	{
		Dead();
		if (m_vwp_abductionSoldier.lock())
		{
			m_vwp_abductionSoldier.lock()->GetGameComponent<Soldier>()->Dead();
		}
	}
}

void ButiEngine::Enemy_Test::CreateJoint()
{
	auto objA = m_vwp_rigidBodyComponent.lock()->GetRigidBody();
	auto objB = m_vwp_abductionSoldier.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody();

	auto joint = ButiBullet::CreateSpringJoint(objA, Matrix4x4(), objB, Matrix4x4(), 0);
	m_vwp_joint = joint;
	m_vwp_joint.lock()->SetLinearUpperLimit(Vector3(1, 1, 1));
	m_vwp_joint.lock()->SetLinearLowerLimit(Vector3(-1, -1, -1));
	m_vwp_joint.lock()->SetAngularUpperLimit(Vector3(0, MathHelper::ToRadian(360), 0));
	m_vwp_joint.lock()->SetLinearStiffness(Vector3(1, 1, 1));
	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetPhysicsManager()->GetActivePhysicsWorld()->
		AddJoint(m_vwp_joint.lock());
}

void ButiEngine::Enemy_Test::DestroyJoint()
{
	if (!m_vwp_joint.lock())
	{
		return;
	}

	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetPhysicsManager()->GetActivePhysicsWorld()->
		RemoveJoint(m_vwp_joint.lock());
}
