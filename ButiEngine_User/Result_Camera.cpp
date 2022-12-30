#include "stdafx_u.h"
#include "Result_Camera.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"

void ButiEngine::Result_Camera::OnUpdate()
{
}

void ButiEngine::Result_Camera::OnSet()
{
}

void ButiEngine::Result_Camera::OnRemove()
{
}

void ButiEngine::Result_Camera::OnShowUI()
{
	if (GUI::Button("ZoomIn"))
	{
		ZoomIn();
	}
	if (GUI::Button("ZoomOut"))
	{
		ZoomOut();
	}
}

void ButiEngine::Result_Camera::Start()
{
	m_startPos = gameObject.lock()->transform->GetLocalPosition();
	m_zoomTargetPos = m_startPos;
	m_zoomTargetPos.z -= 10.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Result_Camera::Clone()
{
	return ObjectFactory::Create<Result_Camera>();
}

void ButiEngine::Result_Camera::ZoomIn()
{
	AddPositionAnimation(m_zoomTargetPos, 120, Easing::EasingType::Liner);
}

void ButiEngine::Result_Camera::ZoomOut()
{
	AddPositionAnimation(m_startPos, 30, Easing::EasingType::EaseOutBack);
}

void ButiEngine::Result_Camera::AddPositionAnimation(const Vector3& arg_targetPos, const std::int32_t arg_animFrame, const Easing::EasingType arg_easeType)
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
	anim->SetEaseType(arg_easeType);
}
