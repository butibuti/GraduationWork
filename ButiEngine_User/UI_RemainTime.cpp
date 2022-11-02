#include "stdafx_u.h"
#include "UI_RemainTime.h"
#include "GameTimer.h"

void ButiEngine::UI_RemainTime::OnUpdate()
{
	m_vwp_textDraw.lock()->SetText("Time:" + std::to_string(m_vwp_gameTimer.lock()->GetRemainSecond()));
}

void ButiEngine::UI_RemainTime::OnSet()
{
}

void ButiEngine::UI_RemainTime::OnRemove()
{
}

void ButiEngine::UI_RemainTime::OnShowUI()
{
}

void ButiEngine::UI_RemainTime::Start()
{
	m_vwp_gameTimer = GetManager().lock()->GetGameObject("GameTimer").lock()->GetGameComponent<GameTimer>();
	m_vwp_textDraw = gameObject.lock()->GetGameComponent<TextDrawComponent>();
	m_vwp_textDraw.lock()->SetText("Time:" + std::to_string(m_vwp_gameTimer.lock()->GetRemainSecond()));
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::UI_RemainTime::Clone()
{
	return ObjectFactory::Create<UI_RemainTime>();
}