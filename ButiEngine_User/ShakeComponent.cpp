#include "stdafx_u.h"
#include "ShakeComponent.h"
#include "PauseManager.h"

void ButiEngine::ShakeComponent::OnUpdate()
{
	Vector3 amplitude = m_startAmplitude * (1.0f - m_vlp_shakeTimer->GetPercent());

	m_theta += m_shakeSpeed * GameDevice::GetWorldSpeed();

	Vector3 rollAngle = (sin(MathHelper::ToRadian(m_theta)) - 0.5f) * 2.0f * amplitude;
	
	Matrix4x4 newRotation = m_startRotation;
	newRotation *= Matrix4x4::RollX(MathHelper::ToRadian(rollAngle.x));
	newRotation *= Matrix4x4::RollY(MathHelper::ToRadian(rollAngle.y));
	newRotation *= Matrix4x4::RollZ(MathHelper::ToRadian(rollAngle.z));

	gameObject.lock()->transform->SetLocalRotation(newRotation);

	if (m_vlp_shakeTimer->Update())
	{
		m_vlp_shakeTimer->Stop();
		gameObject.lock()->transform->SetLocalRotation(m_startRotation);

		Dead();
	}
}

void ButiEngine::ShakeComponent::OnSet()
{
	m_vlp_shakeTimer = ObjectFactory::Create<RelativeTimer>(60);

	m_startRotation = gameObject.lock()->transform->GetLocalRotation();
	m_theta = 45.0f;

	m_vlp_shakeTimer->Start();
}

void ButiEngine::ShakeComponent::OnRemove()
{
}

void ButiEngine::ShakeComponent::OnShowUI()
{
}

void ButiEngine::ShakeComponent::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ShakeComponent::Clone()
{
	return ObjectFactory::Create<ShakeComponent>();
}

void ButiEngine::ShakeComponent::Dead()
{
	ResetRotation();

	SetIsRemove(true);
}

void ButiEngine::ShakeComponent::ResetRotation()
{
	gameObject.lock()->transform->SetLocalRotation(m_startRotation);
}
