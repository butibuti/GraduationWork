#include "stdafx_u.h"
#include "Effect_Belt.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"

void ButiEngine::Effect_Belt::OnUpdate()
{
}

void ButiEngine::Effect_Belt::OnSet()
{
}

void ButiEngine::Effect_Belt::OnRemove()
{
}

void ButiEngine::Effect_Belt::OnShowUI()
{
}

void ButiEngine::Effect_Belt::Start()
{
	if (m_isSpawnToRight)
	{
		Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
		pos.x *= -1.0f;
		pos.y *= -1.0f;
		gameObject.lock()->transform->SetLocalPosition(pos);
	}

	m_startPos = gameObject.lock()->transform->GetLocalPosition();

	Vector3 targetPos = m_startPos;
	targetPos.x = 0.0f;
	AddPositionAnimation(targetPos, 10);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Effect_Belt::Clone()
{
	return ObjectFactory::Create<Effect_Belt>();
}

void ButiEngine::Effect_Belt::Disappear(const std::int32_t arg_disappearFrame)
{
	gameObject.lock()->AddGameComponent<SucideComponent>(arg_disappearFrame);
	AddPositionAnimation(m_startPos, arg_disappearFrame);
}

void ButiEngine::Effect_Belt::AddPositionAnimation(const Vector3& arg_targetPos, const std::int32_t arg_animFrame)
{
	auto anim = gameObject.lock()->GetGameComponent<PositionAnimation>();
	if (anim)
	{
		anim->SetIsRemove(true);
	}

	anim = gameObject.lock()->AddGameComponent<PositionAnimation>();
	anim->SetInitPosition(gameObject.lock()->transform->GetLocalPosition());
	anim->SetTargetPosition(arg_targetPos);
	anim->SetSpeed(1.0f / arg_animFrame);
	anim->SetEaseType(Easing::EasingType::Liner);
}
