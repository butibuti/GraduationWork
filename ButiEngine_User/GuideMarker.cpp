#include "stdafx_u.h"
#include "GuideMarker.h"

void ButiEngine::GuideMarker::OnUpdate()
{
	Vector3 screenPos = GetCamera("UICamera")->WorldToScreen(m_vwp_markTarget.lock()->transform->GetWorldPosition());
	screenPos.x *= -1.0f;
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
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GuideMarker::Clone()
{
	return ObjectFactory::Create<GuideMarker>();
}
