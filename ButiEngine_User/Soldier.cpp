#include "stdafx_u.h"
#include "Soldier.h"
#include "SeparateDrawObject.h"
#include "ScoreManager.h"
#include "SoldierManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Header/GameObjects/DefaultGameComponent/TriggerComponent.h"

float g_maxMoveSpeed = 10.5f;

void ButiEngine::Soldier::OnUpdate()
{
	switch (m_state)
	{
	case ButiEngine::SoldierState::Sleep:
		OnSleep();
		Move();
		break;
	case ButiEngine::SoldierState::Active:
		Move();
		break;
	case ButiEngine::SoldierState::Home:
		Move();
		break;
	case ButiEngine::SoldierState::Abduction:
		break;
	default:
		break;
	}
}

void ButiEngine::Soldier::OnSet()
{
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("Home"))
				{
					if (!m_isInHome)
					{
						if (m_state == SoldierState::Active || m_state == SoldierState::Sleep)
						{
							m_state = SoldierState::Home;
							m_vwp_drawObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color = ButiColor::LightBlue();
						}

						GetManager().lock()->GetGameObject("SoldierManager").lock()->GetGameComponent<SoldierManager>()->AddHomeSoldier(gameObject);

						GetManager().lock()->GetGameObject("ScoreManager").lock()->GetGameComponent<ScoreManager>()->AddScore(100);

						m_isInHome = true;
					}
				}
			}
		}
	);

	gameObject.lock()->AddCollisionLeaveReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("Home"))
				{
					if (m_isInHome)
					{
						if (m_state == SoldierState::Home)
						{
							m_state = SoldierState::Sleep;
							m_vwp_drawObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color = ButiColor::White();
						}

						GetManager().lock()->GetGameObject("SoldierManager").lock()->GetGameComponent<SoldierManager>()->RemoveHomeSoldier(gameObject);

						GetManager().lock()->GetGameObject("ScoreManager").lock()->GetGameComponent<ScoreManager>()->RemoveScore(100);

						m_isInHome = false;
					}
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
	GUI::DragFloat("##maxMoveSpeed", g_maxMoveSpeed, 1.0, 0.0f, 100.0f);
}

void ButiEngine::Soldier::Start()
{
	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	m_vwp_naruko = GetManager().lock()->GetGameObject("Naruko");
	m_vwp_rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
	

	m_state = SoldierState::Sleep;

	m_moveDirection = Vector3Const::Zero;
	m_moveSpeed = 0.0f;

	m_isInHome = false;
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
	if (m_state != SoldierState::Abduction)
	{
		return;
	}

	m_state = SoldierState::Sleep;
}

void ButiEngine::Soldier::Abduction(Value_weak_ptr<GameObject> arg_parent)
{
	if (m_state != SoldierState::Home)
	{
		return;
	}

	m_state = SoldierState::Abduction;
	auto soldierManager = GetManager().lock()->GetGameObject("SoldierManager").lock()->GetGameComponent<SoldierManager>();
	if (soldierManager)
	{
		soldierManager->RemoveHomeSoldier(gameObject);
	}
}

void ButiEngine::Soldier::OnSleep()
{
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 narukoPos = m_vwp_naruko.lock()->transform->GetLocalPosition();

	//ñ¬éqÇ∆ÇÃãóó£Ç™åüímîÕàÕÇÊÇËãﬂÇ©Ç¡ÇΩÇÁäoê¡èÛë‘Ç…Ç∑ÇÈ
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
	SetMoveDirection();
	SetMoveSpeed();

	Vector3 velocity = m_moveDirection * m_moveSpeed;
	m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetVelocity(velocity);
	//m_vwp_rigidBodyComponent.lock()->GetRigidBody()->ApplyForce(velocity);
}

void ButiEngine::Soldier::SetMoveDirection()
{
	if (m_state != SoldierState::Active)
	{
		return;
	}

	Vector3 narukoPos = m_vwp_naruko.lock()->transform->GetLocalPosition();
	Vector3 dir = narukoPos - gameObject.lock()->transform->GetLocalPosition();
	dir.Normalize();

	m_moveDirection.x = dir.x;
	m_moveDirection.y = 0.0f;
	m_moveDirection.z = dir.z;
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
		targetSpeed = g_maxMoveSpeed;
		break;
	case ButiEngine::SoldierState::Home:
		easingSpeed = 0.05f;
		break;
	case ButiEngine::SoldierState::Abduction:
		break;
	default:
		break;
	}

	m_moveSpeed = MathHelper::Lerp(m_moveSpeed, targetSpeed, easingSpeed);
}
