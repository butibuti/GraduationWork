#include "stdafx_u.h"
#include "BlowFriend.h"
#include "FriendBody.h"
#include "BonusFriend.h"
#include "CompleteFriend.h"
#include "FriendHead.h"
#include "Bomb.h"
#include "BombFriend.h"
#include "Header/GameObjects/DefaultGameComponent/RotationAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"

void ButiEngine::BlowFriend::OnUpdate()
{
	m_velocity.y += -m_gravity * GameDevice::GetWorldSpeed();
	Vector3 pos = gameObject.lock()->transform->Translate(m_velocity * GameDevice::GetWorldSpeed());

	if (pos.y < -30.0f)
	{
		auto completeFriend = gameObject.lock()->GetGameComponent<CompleteFriend>();
		if (completeFriend)
		{
			completeFriend->Dead();
		}

		auto bomb = gameObject.lock()->GetGameComponent<BombFriend>();
		if (bomb)
		{
			bomb->Dead();
		}
	}
}

void ButiEngine::BlowFriend::OnSet()
{
	auto completeFriend = gameObject.lock()->GetGameComponent<CompleteFriend>();
	if (completeFriend)
	{
		completeFriend->StopDance();
		completeFriend->StartBlowAnimation();
	}

	auto bonusFriend = gameObject.lock()->GetGameComponent<BonusFriend>();
	if (bonusFriend)
	{
		bonusFriend->SetIsRemove(true);
	}

	SetBlowParam();
}

void ButiEngine::BlowFriend::OnRemove()
{
}

void ButiEngine::BlowFriend::OnShowUI()
{
}

void ButiEngine::BlowFriend::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BlowFriend::Clone()
{
	return ObjectFactory::Create<BlowFriend>();
}

void ButiEngine::BlowFriend::SetBlowParam()
{
	m_velocity = Vector3(0.0f, 1.2f, 0.0f);
	m_gravity = 0.16f;

	Vector3 pos = gameObject.lock()->transform->GetWorldPosition();
	float force = 0.3f;
	float direction = 1.0f;
	if (pos.x > 2.0f)
	{
		direction = 1.0f;
	}
	else if (pos.x < -2.0f)
	{
		direction = -1.0f;
	}
	else
	{
		std::int32_t rand = ButiRandom::GetInt(0, 1);
		direction = rand ? 1.0f : -1.0f;
	}
	m_velocity.x = force * direction;
	if (pos.z > -10.0f)
	{
		m_velocity.x *= 0.2f;
	}

	float rollAngle = ButiRandom::GetRandom(30.0f, 60.0f, 10);
	rollAngle *= direction;
	gameObject.lock()->transform->RollWorldRotation(Quat(Vector3Const::ZAxis, MathHelper::ToRadian(-rollAngle)));
}

void ButiEngine::BlowFriend::AddRollAnimation(float arg_angle)
{
	auto target = gameObject.lock()->transform->Clone();
	target->RollWorldRotation(Quat(-Vector3Const::ZAxis, MathHelper::ToRadian(arg_angle)));

	auto anim = gameObject.lock()->AddGameComponent<RotationAnimation>();
	anim->SetInitRotate(gameObject.lock()->transform->GetWorldRotation());
	anim->SetTargetRotate(target->GetWorldRotation());
	anim->SetSpeed(1.0f / 5);
	anim->SetEaseType(Easing::EasingType::EaseOutExpo);
}
