#include "stdafx_u.h"
#include "Effect_Heart.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"

void ButiEngine::Effect_Heart::OnUpdate()
{
	if (gameObject.lock()->transform->GetLocalPosition().y > 700)
	{
		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::Effect_Heart::OnSet()
{
}

void ButiEngine::Effect_Heart::OnRemove()
{
}

void ButiEngine::Effect_Heart::OnShowUI()
{
}

void ButiEngine::Effect_Heart::Start()
{
	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();
	anim->SetInitPosition(gameObject.lock()->transform->GetLocalPosition());

	Vector3 targetPos = Vector3(0.0f, 710.0f, 0.0f);
	targetPos.x = gameObject.lock()->transform->GetLocalPosition().x;
	targetPos.x += ButiRandom::GetRandom(-300.0f, 300.0f, 10);
	anim->SetTargetPosition(targetPos);

	std::int32_t animFrame = ButiRandom::GetInt(360, 720);
	anim->SetSpeed(1.0f / animFrame);
	anim->SetEaseType(Easing::EasingType::Liner);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Effect_Heart::Clone()
{
	return ObjectFactory::Create<Effect_Heart>();
}
