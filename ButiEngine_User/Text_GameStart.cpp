#include "stdafx_u.h"
#include "Text_GameStart.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"
#include "PauseManager.h"

void ButiEngine::Text_GameStart::OnUpdate()
{
	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return;
	}

	if (m_vlp_lifeTimer->Update())
	{
		m_vlp_lifeTimer->Stop();
		AddAnimation();
	}

	Animation();
}

void ButiEngine::Text_GameStart::OnSet()
{
}

void ButiEngine::Text_GameStart::OnRemove()
{
}

void ButiEngine::Text_GameStart::OnShowUI()
{
	GUI::BulletText("Life");
	GUI::DragInt("##Life", m_life, 1.0f, 0, 100);

	GUI::BulletText("AnimInterval");
	GUI::DragInt("##AnimInterval", m_animIntervalFrame, 1.0f, 0, 100);
}

void ButiEngine::Text_GameStart::Start()
{
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	m_vwp_spriteAnimation = gameObject.lock()->GetGameComponent<SpriteAnimationComponent>();

	m_vlp_animationIntervalTimer = ObjectFactory::Create<RelativeTimer>(m_animIntervalFrame);
	m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>(m_life);

	m_vlp_animationIntervalTimer->Start();
	m_vlp_lifeTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Text_GameStart::Clone()
{
	auto clone = ObjectFactory::Create<Text_GameStart>();
	clone->m_life = m_life;
	clone->m_animIntervalFrame = m_animIntervalFrame;

	return clone;
}

void ButiEngine::Text_GameStart::Animation()
{
	if (m_vlp_animationIntervalTimer->Update())
	{
		m_vwp_spriteAnimation.lock()->UpdateHorizontalAnim(1);
	}
}

void ButiEngine::Text_GameStart::AddAnimation()
{
	auto posAnim = gameObject.lock()->AddGameComponent<PositionAnimation>();

	posAnim->SetInitPosition(gameObject.lock()->transform->GetLocalPosition());
	posAnim->SetTargetPosition(Vector3(0, 480, 0));
	posAnim->SetSpeed(1.0f / 60);
	posAnim->SetEaseType(Easing::EasingType::Liner);

	auto scaleAnim = gameObject.lock()->AddGameComponent<ScaleAnimation>();

	scaleAnim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
	scaleAnim->SetTargetScale(gameObject.lock()->transform->GetLocalScale() * 0.5f);
	scaleAnim->SetSpeed(1.0f / 60);
	scaleAnim->SetEaseType(Easing::EasingType::Liner);
}
