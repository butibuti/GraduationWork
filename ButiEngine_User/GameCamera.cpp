#include "stdafx_u.h"
#include "GameCamera.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "ShakeComponent.h"

void ButiEngine::GameCamera::OnUpdate()
{
	LookAtTarget();

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
	m_vwp_cameraMan = GetManager().lock()->GetGameObject("CameraMan");
	m_startPos = gameObject.lock()->transform->GetLocalPosition();

	m_vlp_waitShakeTimer = ObjectFactory::Create<RelativeTimer>(60);

	m_lookTargetPos = Vector3(0.0f, 0.0f, -100.0f);
	m_zoomStartRotation = Matrix4x4().Identity();
	m_vlp_lookTimer = ObjectFactory::Create<RelativeTimer>(60);

}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameCamera::Clone()
{
	return ObjectFactory::Create<GameCamera>();
}

void ButiEngine::GameCamera::NormalZoom(const std::int32_t arg_zoomInFrame)
{
	auto head = GetManager().lock()->GetGameObject(GameObjectTag("FriendHead"));
	auto headCenter = GetManager().lock()->GetGameObject(GameObjectTag("HeadCenter"));

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

	Vector3 lookTargetPos = headCenter.lock()->transform->GetWorldPosition();

	AddPositionAnimation(targetPos, arg_zoomInFrame);
	StartLookAtTarget(lookTargetPos, arg_zoomInFrame);
}

void ButiEngine::GameCamera::SpecialZoom(const std::int32_t arg_zoomInFrame)
{
	auto head = GetManager().lock()->GetGameObject(GameObjectTag("FriendHead"));
	auto headCenter = GetManager().lock()->GetGameObject(GameObjectTag("HeadCenter"));

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

	Vector3 lookTargetPos = headCenter.lock()->transform->GetWorldPosition();

	AddPositionAnimation(targetPos, arg_zoomInFrame);
	StartLookAtTarget(lookTargetPos, arg_zoomInFrame * 0.25f);

	m_vlp_waitShakeTimer->ChangeCountFrame(arg_zoomInFrame + 1);
	m_vlp_waitShakeTimer->Start();
}

void ButiEngine::GameCamera::ZoomOut(const std::int32_t arg_zoomOutFrame)
{
	Vector3 lookTargetPos = Vector3(0.0f, 0.0f, -100.0f);

	AddPositionAnimation(m_startPos, arg_zoomOutFrame);
	StartLookAtTarget(lookTargetPos, arg_zoomOutFrame * 0.5f);
}

void ButiEngine::GameCamera::StartShake(const std::int32_t arg_shakeFrame)
{
	auto shake = m_vwp_cameraMan.lock()->AddGameComponent<ShakeComponent>();
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
	auto shake = m_vwp_cameraMan.lock()->GetGameComponent<ShakeComponent>();
	if (shake)
	{
		shake->Dead();
	}
}

void ButiEngine::GameCamera::StartLookAtTarget(const Vector3& arg_lookTargetPos, const std::int32_t arg_lookFrame)
{
	m_lookTargetPos = arg_lookTargetPos;
	m_zoomStartRotation = gameObject.lock()->transform->GetLocalRotation();

	m_vlp_lookTimer->ChangeCountFrame(arg_lookFrame);
	m_vlp_lookTimer->Reset();
	m_vlp_lookTimer->Start();
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

	//m_targetPos = arg_animFrame;
	//
	//m_vlp_zoomTimer->ChangeCountFrame(arg_animFrame);
	//m_vlp_zoomTimer->Reset();
	//m_vlp_zoomTimer->Start();
}

void ButiEngine::GameCamera::LookAtTarget()
{
	if (!m_vlp_lookTimer->IsOn())
	{
		gameObject.lock()->transform->SetLookAtRotation(m_lookTargetPos);
		return;
	}

	float progress = m_vlp_lookTimer->GetPercent();
	Quat targetRotation = gameObject.lock()->transform->GetLookAtRotation(m_lookTargetPos, Vector3Const::YAxis).ToQuat();
	Quat newRotation = MathHelper::LearpQuat(m_zoomStartRotation.ToQuat(), targetRotation, progress);
	gameObject.lock()->transform->SetWorldRotation(newRotation.ToMatrix());

	if (m_vlp_lookTimer->Update())
	{
		m_vlp_lookTimer->Stop();

		gameObject.lock()->transform->SetLookAtRotation(m_lookTargetPos);
	}
}
