#include "stdafx_u.h"
#include "Effect_Explosion.h"

void ButiEngine::Effect_Explosion::OnUpdate()
{
	if (m_vlp_life->Update()) {
		gameObject.lock()->SetIsRemove(true);
		m_vlp_smokeParent->SetIsRemove(true);
		m_vlp_needleParent->SetIsRemove(true);
	}

}

void ButiEngine::Effect_Explosion::OnShowUI()
{
}

void ButiEngine::Effect_Explosion::Start()
{
	m_vlp_life = ObjectFactory::Create<RelativeTimer>(30.0f);
	m_vlp_smokeParent = AddObjectFromCereal("SmokeParent");
	m_vlp_needleParent = AddObjectFromCereal("NeedleParent");
	m_vlp_smokeParent->transform->SetBaseTransform(gameObject.lock()->transform);
	m_vlp_needleParent->transform->SetBaseTransform(gameObject.lock()->transform);
	m_vlp_life->Start();
}

ButiEngine::Value_ptr< ButiEngine::GameComponent> ButiEngine::Effect_Explosion::Clone()
{
	auto output = ObjectFactory::Create < Effect_Explosion >();
	return output;
}
