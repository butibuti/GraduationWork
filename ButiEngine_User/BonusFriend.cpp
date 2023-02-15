#include "stdafx_u.h"
#include "BonusFriend.h"
#include "FriendManager.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/RotationAnimationComponent.h"
#include "PauseManager.h"
#include "CompleteFriend.h"

void ButiEngine::BonusFriend::OnUpdate()
{
	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return;
	}

	if (m_isRotate)
	{
		Rotate();
	}

	if (m_isMoveBack)
	{
		MoveBack();
	}
}

void ButiEngine::BonusFriend::OnSet()
{
}

void ButiEngine::BonusFriend::OnRemove()
{
}

void ButiEngine::BonusFriend::OnShowUI()
{
}

void ButiEngine::BonusFriend::Start()
{
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	m_vlp_moveBackTimer = ObjectFactory::Create<RelativeTimer>(90);

	m_isMoveBack = false;
	m_isRotate = false;
	m_isStopRotate = false;

	auto completeFriendComponent = gameObject.lock()->GetGameComponent<CompleteFriend>();
	m_vwp_head = completeFriendComponent->GetHead();
	m_vwp_body = completeFriendComponent->GetBody();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BonusFriend::Clone()
{
	return ObjectFactory::Create<BonusFriend>();
}

void ButiEngine::BonusFriend::MoveBack()
{
	float progress = m_vlp_moveBackTimer->GetPercent();
	Vector3 newPos = MathHelper::LerpPosition(m_moveBackStartPos, m_moveBackTargetPos, progress);

	gameObject.lock()->transform->SetLocalPosition(newPos);

	if (m_vlp_moveBackTimer->Update())
	{
		m_vlp_moveBackTimer->Stop();
		m_isMoveBack = false;
		m_isStopRotate = true;

		gameObject.lock()->transform->SetLocalPosition(m_moveBackTargetPos);
	}
}

void ButiEngine::BonusFriend::Rotate()
{
	float rotateSpeed = 10.0f;
	gameObject.lock()->transform->RollLocalRotationY_Degrees(rotateSpeed * GameDevice::GetWorldSpeed());

	if (m_isStopRotate)
	{
		if (IsFrontHead())
		{
			StopRotate();
		}
	}
}

void ButiEngine::BonusFriend::StopRotate()
{
	m_isRotate = false;

	auto target = gameObject.lock()->transform->Clone();
	float rollAngle = GetLookForwardHeadAngle();
	target->RollLocalRotationY_Degrees(rollAngle);

	auto anim = gameObject.lock()->AddGameComponent<RotationAnimation>();
	anim->SetInitRotate(gameObject.lock()->transform->GetLocalRotation());
	anim->SetTargetRotate(target->GetLocalRotation());
	anim->SetSpeed(1.0f / 10);
	anim->SetEaseType(Easing::EasingType::EaseOutQuad);
}

float ButiEngine::BonusFriend::GetLookForwardHeadAngle()
{
	Vector3 headFront = GetFrontXZ(m_vwp_head.lock()->transform->GetFront());
	float rollAngle = abs(MathHelper::ToDegree(std::acos(headFront.Dot(Vector3Const::ZAxis))));
	bool isClockwise = headFront.x >= 0.0f;
	if (isClockwise)
	{
		rollAngle *= -1.0f;
	}

	return rollAngle;
}

ButiEngine::Vector3 ButiEngine::BonusFriend::GetFrontXZ(const Vector3& arg_front)
{
	Vector3 front = arg_front;
	front.y = 0.0f;
	if (front == Vector3Const::Zero)
	{
		front.z = 1.0f;
	}
	front.Normalize();

	return front;
}

bool ButiEngine::BonusFriend::IsFrontHead()
{
	if (!m_vwp_head.lock())
	{
		return false;
	}

	Vector3 front = GetFrontXZ(m_vwp_head.lock()->transform->GetFront());

	float angle = abs(MathHelper::ToDegree(std::acos(front.Dot(Vector3Const::ZAxis))));

	return angle <= m_frontBorder;
}

void ButiEngine::BonusFriend::Appear(const std::int32_t arg_friendNum)
{
	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	anim->SetInitPosition(pos);

	Vector3 targetPos = pos;
	if (arg_friendNum == 0)
	{
		targetPos.x -= 2.0f;
	}
	else if (arg_friendNum == 1)
	{
		targetPos.x += 2.0f;
	}
	else if (arg_friendNum == 2)
	{
		targetPos.y += 2.0f;
		targetPos.z -= 2.0f;
	}

	anim->SetTargetPosition(targetPos);
	
	std::int32_t appearFrame = 15;
	anim->SetSpeed(1.0f / appearFrame);
	anim->SetEaseType(Easing::EasingType::EaseOutBack);
}

void ButiEngine::BonusFriend::StartMoveBack()
{
	gameObject.lock()->GetGameComponent<CompleteFriend>()->StartDance();

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	m_moveBackStartPos = pos;

	m_moveBackTargetPos.x = ButiRandom::GetInt(-7, 7);
	m_moveBackTargetPos.y = 0.0f;
	m_moveBackTargetPos.z = pos.z + (ButiRandom::GetInt(-70, -45));

	m_isMoveBack = true;
	m_isRotate = true;
	m_vlp_moveBackTimer->Start();
}
