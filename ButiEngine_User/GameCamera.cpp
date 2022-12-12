#include "stdafx_u.h"
#include "GameCamera.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "ShakeComponent.h"

void ButiEngine::GameCamera::OnUpdate()
{
	if (m_vlp_waitShakeTimer->Update())
	{
		m_vlp_waitShakeTimer->Stop();
		StartShake(10);
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
	StopShake();

	auto head = GetManager().lock()->GetGameObject(GameObjectTag("FriendHead"));

	auto headCenter = GetManager().lock()->GetGameObject(GameObjectTag("HeadCenter"));
	m_vwp_lookAt.lock()->SetLookTarget(headCenter.lock()->transform);

	Vector3 centerPos = headCenter.lock()->transform->GetWorldPosition();
	Vector3 headFront = head.lock()->transform->GetFront();
	headFront.y = 0.0f;
	if (headFront == Vector3Const::Zero)
	{
		headFront.z = 1.0f;
	}
	headFront.Normalize();
	Vector3 targetPos = centerPos + headFront * 55.0f;
	targetPos.x = min(targetPos.x, 5.0f);
	targetPos.x = max(targetPos.x, -5.0f);
	targetPos.z = max(targetPos.z, 50.0f);

	AddPositionAnimation(targetPos, arg_zoomInFrame);
}

void ButiEngine::GameCamera::SpecialZoom(const std::int32_t arg_zoomInFrame)
{
	StopShake();

	auto head = GetManager().lock()->GetGameObject(GameObjectTag("FriendHead"));

	auto headCenter = GetManager().lock()->GetGameObject(GameObjectTag("HeadCenter"));
	m_vwp_lookAt.lock()->SetLookTarget(headCenter.lock()->transform);

	Vector3 centerPos = headCenter.lock()->transform->GetWorldPosition();
	Vector3 headFront = head.lock()->transform->GetFront();
	headFront.y = 0.0f;
	if (headFront == Vector3Const::Zero)
	{
		headFront.z = 1.0f;
	}
	headFront.Normalize();
	Vector3 targetPos = centerPos + headFront * 15.0f;

	Vector3 scattering;
	float radius = 3.0f;
	scattering.x = ButiRandom::GetRandom(-radius, radius, 10);
	scattering.y = ButiRandom::GetRandom(-radius, radius, 10);
	scattering.z = 0.0f;
	targetPos += scattering;

	AddPositionAnimation(targetPos, arg_zoomInFrame);

	m_vlp_waitShakeTimer->ChangeCountFrame(arg_zoomInFrame + 5);
	m_vlp_waitShakeTimer->Start();
}

void ButiEngine::GameCamera::ZoomOut(const std::int32_t arg_zoomOutFrame)
{
	StopShake();

	auto defaultLookObject = GetManager().lock()->GetGameObject(GameObjectTag("DefaultLookObject"));
	m_vwp_lookAt.lock()->SetLookTarget(defaultLookObject.lock()->transform);

	AddPositionAnimation(m_startPos, arg_zoomOutFrame);
}

void ButiEngine::GameCamera::StartShake(const std::int32_t arg_shakeFrame)
{
	auto shake = gameObject.lock()->AddGameComponent<ShakeComponent>();
	shake->SetShakeFrame(arg_shakeFrame);
	shake->SetShakeSpeed(90.0f);
	Vector3 amplitude;
	amplitude.x = ButiRandom::GetRandom(0.1f, 0.15f, 10);
	amplitude.y = ButiRandom::GetRandom(0.1f, 0.15f, 10);
	amplitude.z = ButiRandom::GetRandom(0.1f, 0.15f, 10);
	shake->SetStartAmplitude(amplitude);
}

void ButiEngine::GameCamera::StopShake()
{
	auto shake = gameObject.lock()->GetGameComponent<ShakeComponent>();
	if (shake)
	{
		shake->Dead();
	}
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
	anim->SetEaseType(Easing::EasingType::Liner);
}
