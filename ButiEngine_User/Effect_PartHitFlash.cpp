#include "stdafx_u.h"
#include "Effect_PartHitFlash.h"
#include "PauseManager.h"

void ButiEngine::Effect_PartHitFlash::OnUpdate()
{
	float progress = m_vlp_lifeTImer->GetPercent();
	float multiply = MathHelper::Lerp(1.0f, 1.1f, progress);

	Vector3 newScale = gameObject.lock()->transform->GetLocalScale();
	newScale *= multiply;
	gameObject.lock()->transform->SetLocalScale(newScale);

	if (m_vlp_lifeTImer->Update())
	{
		m_vlp_lifeTImer->Stop();
		Vector3 newScale = gameObject.lock()->transform->GetLocalScale();
		newScale *= 1.1f;
		gameObject.lock()->transform->SetLocalScale(newScale);

		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::Effect_PartHitFlash::OnSet()
{
}

void ButiEngine::Effect_PartHitFlash::OnRemove()
{
}

void ButiEngine::Effect_PartHitFlash::OnShowUI()
{
}

void ButiEngine::Effect_PartHitFlash::Start()
{
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	gameObject.lock()->transform->RollLocalRotationZ_Degrees(ButiRandom::GetInt(0, 359));

	m_vlp_lifeTImer = ObjectFactory::Create<RelativeTimer>(6);
	m_vlp_lifeTImer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Effect_PartHitFlash::Clone()
{
	return ObjectFactory::Create<Effect_PartHitFlash>();
}
