#include "stdafx_u.h"
#include "Text_OK.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::Text_OK::OnUpdate()
{
    if (m_vlp_appearTimer->Update())
    {
        m_vlp_appearTimer->Stop();
        m_vlp_lifeTimer->Start();

        Vector3 targetScale = gameObject.lock()->transform->GetLocalScale() * 0.9f;
        AddScaleAnimation(targetScale, 85, Easing::EasingType::Liner);
    }

    if (m_vlp_lifeTimer->Update())
    {
        m_vlp_lifeTimer->Stop();
        std::int32_t animFrame = 10;
        gameObject.lock()->AddGameComponent<SucideComponent>(animFrame);
        AddScaleAnimation(0.0f, animFrame, Easing::EasingType::EaseOutExpo);
    }
}

void ButiEngine::Text_OK::OnSet()
{
}

void ButiEngine::Text_OK::OnRemove()
{
}

void ButiEngine::Text_OK::OnShowUI()
{
}

void ButiEngine::Text_OK::Start()
{
    m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>(85);

    m_scaleMagnification = 1.0f;

    Vector3 targetScale = Vector3(300, 200, 1);
    std::int32_t appearFrame = 10;
    AddScaleAnimation(targetScale, appearFrame, Easing::EasingType::EaseOutQuad);

    m_vlp_appearTimer = ObjectFactory::Create<RelativeTimer>(appearFrame);
    m_vlp_appearTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Text_OK::Clone()
{
	return ObjectFactory::Create<Text_OK>();
}

void ButiEngine::Text_OK::AddScaleAnimation(const Vector3& arg_targetScale, const std::int32_t arg_animFrame, const Easing::EasingType arg_easeType)
{
    auto anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();

    anim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
    anim->SetTargetScale(arg_targetScale);
    anim->SetSpeed(1.0f / arg_animFrame);
    anim->SetEaseType(arg_easeType);
}
