#include "stdafx_u.h"
#include "GameCamera.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "ShakeComponent.h"

void ButiEngine::GameCamera::OnUpdate()
{
	if (m_vlp_waitShakeTimer->Update())
	{
		m_vlp_waitShakeTimer->Stop();
		Shake(20);
	}
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

	m_vlp_waitShakeTimer = ObjectFactory::Create<RelativeTimer>(60);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameCamera::Clone()
{
	return ObjectFactory::Create<GameCamera>();
}

void ButiEngine::GameCamera::NormalZoom(const std::int32_t arg_zoomInFrame)
{
	auto headCenter = GetManager().lock()->GetGameObject(GameObjectTag("HeadCenter"));
	m_vwp_lookAt.lock()->SetLookTarget(headCenter.lock()->transform);

	Vector3 dir = (headCenter.lock()->transform->GetLocalPosition() - m_startPos).Normalize();
	Vector3 targetPos = m_startPos + dir * 1.5f;

	AddPositionAnimation(targetPos, arg_zoomInFrame);

	m_vlp_waitShakeTimer->ChangeCountFrame(arg_zoomInFrame - 20);
	m_vlp_waitShakeTimer->Start();
}

void ButiEngine::GameCamera::SpecialZoom(const std::int32_t arg_zoomInFrame)
{
	auto headCenter = GetManager().lock()->GetGameObject(GameObjectTag("HeadCenter"));
	m_vwp_lookAt.lock()->SetLookTarget(headCenter.lock()->transform);

	Vector3 dir = (headCenter.lock()->transform->GetLocalPosition() - m_startPos).Normalize();
	Vector3 targetPos = m_startPos + dir * 2.0f;
	Vector3 scattering;
	float radius = 1.0f;
	scattering.x = ButiRandom::GetRandom(-radius, radius, 10);
	scattering.y = ButiRandom::GetRandom(-radius, radius, 10);
	scattering.z = 0.0f;
	targetPos += scattering;

	AddPositionAnimation(targetPos, arg_zoomInFrame);

	m_vlp_waitShakeTimer->ChangeCountFrame(arg_zoomInFrame - 20);
	m_vlp_waitShakeTimer->Start();
}

void ButiEngine::GameCamera::ZoomOut(const std::int32_t arg_zoomOutFrame)
{
	auto defaultLookObject = GetManager().lock()->GetGameObject(GameObjectTag("DefaultLookObject"));
	m_vwp_lookAt.lock()->SetLookTarget(defaultLookObject.lock()->transform);

	AddPositionAnimation(m_startPos, arg_zoomOutFrame);
}

void ButiEngine::GameCamera::Shake(const std::int32_t arg_shakeFrame)
{
	auto shake = gameObject.lock()->AddGameComponent<ShakeComponent>();
	shake->SetShakeFrame(arg_shakeFrame);
	shake->SetShakeSpeed(90.0f);
	Vector3 amplitude;
	amplitude.x = ButiRandom::GetRandom(1.0f, 2.0f, 10);
	amplitude.y = ButiRandom::GetRandom(1.0f, 2.0f, 10);
	amplitude.z = ButiRandom::GetRandom(1.0f, 2.0f, 10);
	shake->SetStartAmplitude(amplitude);
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
