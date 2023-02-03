#include "stdafx_u.h"
#include "PartStickAnimation.h"
#include "PauseManager.h"

void ButiEngine::PartStickAnimation::OnUpdate()
{
	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return;
	}

	float amplitude = m_startAmplitude * (1.0f - m_vlp_aniimationTimer->GetPercent());

	m_theta += m_animationSpeed * GameDevice::GetWorldSpeed();

	float magnification = 1.0f + ((sin(MathHelper::ToRadian(m_theta)) - 0.5f) * 2.0f) * amplitude;
	Vector3 newScale = m_standardScale * magnification;

	gameObject.lock()->transform->SetLocalScale(newScale);

	if (m_vlp_aniimationTimer->Update())
	{
		m_vlp_aniimationTimer->Stop();
		gameObject.lock()->transform->SetLocalScale(m_standardScale);
		SetIsRemove(true);
	}
}

void ButiEngine::PartStickAnimation::OnSet()
{
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	m_vlp_aniimationTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_standardScale = gameObject.lock()->transform->GetLocalScale();
	m_startAmplitude = 0.1f;
	m_theta = 45.0f;
	m_animationSpeed = 30.0f;

	m_vlp_aniimationTimer->Start();
}

void ButiEngine::PartStickAnimation::OnRemove()
{
	gameObject.lock()->transform->SetLocalScale(m_standardScale);
}

void ButiEngine::PartStickAnimation::OnShowUI()
{
}

void ButiEngine::PartStickAnimation::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::PartStickAnimation::Clone()
{
	return ObjectFactory::Create<PartStickAnimation>();
}
