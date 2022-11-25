#include "stdafx_u.h"
#include "GameCamera.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"

void ButiEngine::GameCamera::OnUpdate()
{
}

void ButiEngine::GameCamera::OnSet()
{
}

void ButiEngine::GameCamera::OnRemove()
{
}

void ButiEngine::GameCamera::OnShowUI()
{
}

void ButiEngine::GameCamera::Start()
{
	m_vwp_lookAt = gameObject.lock()->GetGameComponent<LookAtComponent>();
	m_startPos = gameObject.lock()->transform->GetLocalPosition();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameCamera::Clone()
{
	return ObjectFactory::Create<GameCamera>();
}

void ButiEngine::GameCamera::NormalZoom(const std::int32_t arg_zoomInFrame)
{
	auto friendBody = GetManager().lock()->GetGameObject(GameObjectTag("FriendBody"));
	m_vwp_lookAt.lock()->SetLookTarget(friendBody.lock()->transform);

	Vector3 dir = (friendBody.lock()->transform->GetLocalPosition() - m_startPos).Normalize();
	Vector3 targetPos = m_startPos + dir * 1.5f;

	AddPositionAnimation(targetPos, arg_zoomInFrame);

	std::int32_t shakeFrame = arg_zoomInFrame;
	StartShake(shakeFrame);
}

void ButiEngine::GameCamera::SpecialZoom(const std::int32_t arg_zoomInFrame)
{
	auto friendBody = GetManager().lock()->GetGameObject(GameObjectTag("FriendBody"));
	m_vwp_lookAt.lock()->SetLookTarget(friendBody.lock()->transform);

	Vector3 dir = (friendBody.lock()->transform->GetLocalPosition() - m_startPos).Normalize();
	Vector3 targetPos = m_startPos + dir * 1.5f;

	AddPositionAnimation(targetPos, arg_zoomInFrame);

	std::int32_t shakeFrame = arg_zoomInFrame;
	StartShake(shakeFrame);
}

void ButiEngine::GameCamera::ZoomOut(const std::int32_t arg_zoomOutFrame)
{
	auto defaultLookObject = GetManager().lock()->GetGameObject(GameObjectTag("DefaultLookObject"));
	m_vwp_lookAt.lock()->SetLookTarget(defaultLookObject.lock()->transform);

	AddPositionAnimation(m_startPos, arg_zoomOutFrame);
}

void ButiEngine::GameCamera::AddPositionAnimation(const Vector3& arg_targetPos, const std::int32_t arg_animFrame)
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
	anim->SetEaseType(Easing::EasingType::EaseOutExpo);
}

void ButiEngine::GameCamera::StartShake(const std::int32_t arg_shakeFrame)
{
}
