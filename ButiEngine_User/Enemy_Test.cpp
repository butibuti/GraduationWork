#include "stdafx_u.h"
#include "Enemy_Test.h"
#include "Soldier.h"
#include "SeparateDrawObject.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

void ButiEngine::Enemy_Test::OnUpdate()
{
	Vector2 moveDirection;
	Vector3 homePos = m_vwp_home.lock()->transform->GetLocalPosition();
	Vector3 dir = homePos - gameObject.lock()->transform->GetLocalPosition();
	dir.Normalize();

	moveDirection.x = dir.x;
	moveDirection.y = dir.z;


	Vector3 velocity = Vector3(moveDirection.x, 0.0f, moveDirection.y) * 1.0f;
	m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetVelocity(velocity * GameDevice::GetWorldSpeed());
}

void ButiEngine::Enemy_Test::OnSet()
{
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				//ƒ^ƒO”»’è
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("Soldier"))
				{
					auto soldierComponent = arg_other.vwp_gameObject.lock()->GetGameComponent<Soldier>();
					auto soldierState = soldierComponent->GetState();
					switch (soldierState)
					{
					case ButiEngine::SoldierState::Sleep:
						break;
					case ButiEngine::SoldierState::Active:
						Dead();
						break;
					case ButiEngine::SoldierState::Home:
						soldierComponent->Dead();
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
	m_vwp_home = GetManager().lock()->GetGameObject("Home");
	m_vwp_rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Enemy_Test::Clone()
{
	return ObjectFactory::Create<Enemy_Test>();
}

void ButiEngine::Enemy_Test::Dead()
{
	gameObject.lock()->SetIsRemove(true);
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
}
