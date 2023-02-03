#include "stdafx_u.h"
#include "GuideMarker.h"

void ButiEngine::GuideMarker::OnUpdate()
{
	Vector3 screenPos = GetCamera("main")->WorldToScreen(m_vwp_markTarget.lock()->transform->GetWorldPosition());
	
	screenPos.z = gameObject.lock()->transform->GetLocalPosition().z;

	gameObject.lock()->transform->SetLocalPosition(screenPos);

	gameObject.lock()->transform->RollLocalRotationZ_Degrees(3.0f);
}

void ButiEngine::GuideMarker::OnSet()
{
}

void ButiEngine::GuideMarker::OnRemove()
{
}

void ButiEngine::GuideMarker::OnShowUI()
{
}

void ButiEngine::GuideMarker::Start()
{
	if (m_vwp_markTarget.lock())
	{
		Vector3 screenPos = GetCamera("main")->WorldToScreen(m_vwp_markTarget.lock()->transform->GetWorldPosition());

		screenPos.z = gameObject.lock()->transform->GetLocalPosition().z;

		gameObject.lock()->transform->SetLocalPosition(screenPos);
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GuideMarker::Clone()
{
	return ObjectFactory::Create<GuideMarker>();
}
