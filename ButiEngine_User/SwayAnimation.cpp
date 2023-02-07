#include "stdafx_u.h"
#include "SwayAnimation.h"
#include "PauseManager.h"

void ButiEngine::SwayAnimation::OnUpdate()
{
	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return;
	}

	float progress = m_vlp_animTimer->GetPercent();
	float angle = MathHelper::Lerp(m_startAngle, m_targetAngle, progress);

	gameObject.lock()->transform->SetLocalRotationZ_Degrees(angle);

	if (m_vlp_animTimer->Update())
	{
		angle = m_targetAngle;
		gameObject.lock()->transform->SetLocalRotationZ_Degrees(angle);

		m_startAngle = m_targetAngle;
		m_targetAngle *= -1.0f;
	}
}

void ButiEngine::SwayAnimation::OnSet()
{
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	m_startAngle = 0.0f;
	m_targetAngle = ButiRandom::GetRandom(30.0f, 50.0f);
	if (ButiRandom::GetInt(0, 1))
	{
		m_targetAngle *= -1.0f;
	}

	m_vlp_animTimer = ObjectFactory::Create<RelativeTimer>(ButiRandom::GetInt(40, 70));
	m_vlp_animTimer->Start();
}

void ButiEngine::SwayAnimation::OnRemove()
{
}

void ButiEngine::SwayAnimation::OnShowUI()
{
}

void ButiEngine::SwayAnimation::Start()
{
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	m_startAngle = 0.0f;
	m_targetAngle = ButiRandom::GetRandom(10.0f, 30.0f);
	if (ButiRandom::GetInt(0, 1))
	{
		m_targetAngle *= -1.0f;
	}

	m_vlp_animTimer = ObjectFactory::Create<RelativeTimer>(ButiRandom::GetInt(40, 70));
	m_vlp_animTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SwayAnimation::Clone()
{
	return ObjectFactory::Create<SwayAnimation>();
}
