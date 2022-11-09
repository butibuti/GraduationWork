#include "stdafx_u.h"
#include "RemoveMeshDraw.h"

void ButiEngine::RemoveMeshDraw::OnUpdate()
{
}

void ButiEngine::RemoveMeshDraw::OnSet()
{
}

void ButiEngine::RemoveMeshDraw::OnRemove()
{
}

void ButiEngine::RemoveMeshDraw::OnShowUI()
{
}

void ButiEngine::RemoveMeshDraw::Start()
{
	//MeshDrawComponent‚ğ‚·‚×‚ÄÁ‚µ‚Ä‚±‚ÌComponent©g‚àÁ‚·
	for (std::int32_t i = 0; i < 1; i++) {
		auto meshDrawComponent = gameObject.lock()->GetGameComponent<MeshDrawComponent>(i);
		
		if (meshDrawComponent)
		{
			meshDrawComponent->SetIsRemove(true);
		}
	}

	SetIsRemove(true);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::RemoveMeshDraw::Clone()
{
	return ObjectFactory::Create<RemoveMeshDraw>();
}
