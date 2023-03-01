#include "stdafx_u.h"
#include "SeparateDrawObject.h"

void ButiEngine::SeparateDrawObject::OnUpdate()
{
}

void ButiEngine::SeparateDrawObject::OnSet()
{
}

void ButiEngine::SeparateDrawObject::OnRemove()
{
}

void ButiEngine::SeparateDrawObject::OnShowUI()
{
}

void ButiEngine::SeparateDrawObject::Start()
{
	//DrawObjectì¬
	std::string objectName = "DrawObject_" + gameObject.lock()->GetGameObjectTags()[0].GetID();

	m_vwp_drawObject = GetManager().lock()->AddObjectFromCereal(objectName);
	m_vwp_drawObject.lock()->SetObjectName("DrawObject_" + gameObject.lock()->GetGameObjectName());
	m_vwp_drawObject.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);

	for (std::int32_t i = 0; i < 1; i++) {
		//GameObject‚©‚çMeshDrawComponent‚Ìî•ñ‚ðŽ‚Á‚Ä‚«‚ÄDrawObject‚É’Ç‰Á
		auto meshDrawComponent = gameObject.lock()->GetGameComponent<MeshDrawComponent>(i);
		m_vwp_drawObject.lock()->AddGameComponent(meshDrawComponent->Clone());

		//’Ç‰ÁŒãGameObject‚É‚Â‚¢‚Ä‚¢‚éMeshDrawComponent‚Ííœ
		meshDrawComponent->SetIsRemove(true);
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SeparateDrawObject::Clone()
{
	return ObjectFactory::Create<SeparateDrawObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::SeparateDrawObject::GetDrawObject()
{
	return m_vwp_drawObject;
}

void ButiEngine::SeparateDrawObject::ReturnDrawObject()
{
	auto meshDrawComponent = m_vwp_drawObject.lock()->GetGameComponent<MeshDrawComponent>();
	gameObject.lock()->AddGameComponent(meshDrawComponent->Clone());
	m_vwp_drawObject.lock()->SetIsRemove(true);
}

void ButiEngine::SeparateDrawObject::Dead()
{
	if (m_vwp_drawObject.lock())
	{
		m_vwp_drawObject.lock()->SetIsRemove(true);
	}
}
