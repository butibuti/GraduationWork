#include "stdafx_u.h"
#include "ConcentratedLine.h"

void ButiEngine::ConcentratedLine::OnUpdate()
{
	gameObject.lock()->transform->RollLocalRotationZ_Degrees(30.0f);

	if (m_vlp_lifeTimer->Update())
	{
		m_vwp_parent.lock()->SetIsRemove(true);
		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::ConcentratedLine::OnSet()
{
}

void ButiEngine::ConcentratedLine::OnRemove()
{
}

void ButiEngine::ConcentratedLine::OnShowUI()
{
}

void ButiEngine::ConcentratedLine::Start()
{
	m_vwp_parent = GetManager().lock()->AddObjectFromCereal("ConcentratedLineParent");
	gameObject.lock()->transform->SetBaseTransform(m_vwp_parent.lock()->transform);
	gameObject.lock()->transform->SetLocalPosition(Vector3Const::Zero);

	m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>(m_lifeTime);
	m_vlp_lifeTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ConcentratedLine::Clone()
{
	return ObjectFactory::Create<ConcentratedLine>();
}
