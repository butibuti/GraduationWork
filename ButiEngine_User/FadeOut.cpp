#include "stdafx_u.h"
#include "FadeOut.h"

void ButiEngine::FadeOut::OnUpdate()
{
	float progress = m_vlp_timer->GetPercent();
	float newAlpha = MathHelper::Lerp(m_startAlpha, 0.0f, progress);
	
	Vector4 newColor = m_vwp_meshDraw.lock()->GetColor();
	newColor.w = newAlpha;
	m_vwp_meshDraw.lock()->SetColor(newColor);

	if (m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();

		m_vwp_meshDraw.lock()->SetColor(Vector4(0, 0, 0, 0));
		SetIsRemove(true);
	}
}

void ButiEngine::FadeOut::OnSet()
{
	m_vwp_meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	
	m_startAlpha = m_vwp_meshDraw.lock()->GetColor().w;

	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(m_fadeFrame);
	m_vlp_timer->Start();
}

void ButiEngine::FadeOut::OnRemove()
{
}

void ButiEngine::FadeOut::OnShowUI()
{
}

void ButiEngine::FadeOut::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FadeOut::Clone()
{
	return ObjectFactory::Create<FadeOut>();
}
