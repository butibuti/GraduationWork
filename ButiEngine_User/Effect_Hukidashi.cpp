#include "stdafx_u.h"
#include "Effect_Hukidashi.h"

void ButiEngine::Effect_Hukidashi::OnUpdate()
{
    if (m_vlp_lifeTimer->Update())
    {
        gameObject.lock()->SetIsRemove(true);
    }
}

void ButiEngine::Effect_Hukidashi::OnSet()
{
}

void ButiEngine::Effect_Hukidashi::OnRemove()
{
}

void ButiEngine::Effect_Hukidashi::OnShowUI()
{
}

void ButiEngine::Effect_Hukidashi::Start()
{
    m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>(60);
    m_vlp_lifeTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Effect_Hukidashi::Clone()
{
    return ObjectFactory::Create<Effect_Hukidashi>();
}
