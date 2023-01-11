#include "stdafx_u.h"
#include "SpriteAnimation_DestroyOwn.h"
#include "Header/GameObjects/DefaultGameComponent/SpriteAnimationComponent.h"

void ButiEngine::SpriteAnimation_DestroyOwn::OnUpdate()
{
	Animation();
}

void ButiEngine::SpriteAnimation_DestroyOwn::OnSet()
{
}

void ButiEngine::SpriteAnimation_DestroyOwn::OnShowUI()
{
	GUI::BulletText("AnimInterval");
	GUI::DragInt("##AnimInterval", m_animIntervalFrame, 1.0f, 0, 100);
	
	GUI::BulletText("LoopCount");
	GUI::DragInt("##LoopCount", m_maxLoopCount, 1.0f, 0, 100);
}

void ButiEngine::SpriteAnimation_DestroyOwn::Start()
{
	m_vwp_spriteAnimation = gameObject.lock()->GetGameComponent<SpriteAnimationComponent>();

	m_vlp_animationIntervalTimer = ObjectFactory::Create<RelativeTimer>(m_animIntervalFrame);
	m_vlp_animationIntervalTimer->Start();

	m_loopCount = 0;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SpriteAnimation_DestroyOwn::Clone()
{
	auto clone = ObjectFactory::Create<SpriteAnimation_DestroyOwn>();
	clone->m_maxLoopCount = m_maxLoopCount;
	clone->m_animIntervalFrame = m_animIntervalFrame;
	return clone;
}

void ButiEngine::SpriteAnimation_DestroyOwn::Animation()
{
	if (m_vlp_animationIntervalTimer->Update())
	{
		m_vwp_spriteAnimation.lock()->UpdateHorizontalAnim(1);

		//アニメーションが一周したら死ぬ
		if (m_vwp_spriteAnimation.lock()->GetHorizontalAnim() == m_vwp_spriteAnimation.lock()->GetHorizontalSplitScale() - 1)
		{
			m_loopCount++;
			if (m_loopCount == m_maxLoopCount)
			{
				gameObject.lock()->SetIsRemove(true);
			}
		}
	}
}
