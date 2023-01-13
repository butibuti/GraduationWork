#include "stdafx_u.h"
#include "Effect_Hukidashi.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::Effect_Hukidashi::OnUpdate()
{
    if (m_vlp_appearTimer->Update())
    {
        m_vlp_appearTimer->Stop();
        m_vlp_lifeTimer->Start();

        Vector3 targetScale = gameObject.lock()->transform->GetLocalScale() * 0.9f;
        AddScaleAnimation(targetScale, 80, Easing::EasingType::Liner);
    }

    if (m_vlp_lifeTimer->Update())
    {
        m_vlp_lifeTimer->Stop();
        std::int32_t animFrame = 10;
        gameObject.lock()->AddGameComponent<SucideComponent>(animFrame);
        AddScaleAnimation(0.0f, animFrame, Easing::EasingType::EaseOutExpo);
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
    m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>(80);

    m_scaleMagnification = 1.0f;

    Vector3 targetScale = gameObject.lock()->transform->GetLocalScale();
    targetScale *= 1.1f;
    std::int32_t appearFrame = 10;
    AddScaleAnimation(targetScale, appearFrame, Easing::EasingType::Parabola);

    m_vlp_appearTimer = ObjectFactory::Create<RelativeTimer>(appearFrame);
    m_vlp_appearTimer->Start();

    auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/Bonus.wav"));
    GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Effect_Hukidashi::Clone()
{
    return ObjectFactory::Create<Effect_Hukidashi>();
}

void ButiEngine::Effect_Hukidashi::AddScaleAnimation(const Vector3& arg_targetScale, const std::int32_t arg_animFrame, const Easing::EasingType arg_easeType)
{
    auto anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();

    anim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
    anim->SetTargetScale(arg_targetScale);
    anim->SetSpeed(1.0f / arg_animFrame);
    anim->SetEaseType(arg_easeType);
}
