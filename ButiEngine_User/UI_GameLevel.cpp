#include "stdafx_u.h"
#include "UI_GameLevel.h"
#include "GameLevelManager.h"

void ButiEngine::UI_GameLevel::OnUpdate()
{
	m_vwp_textDraw.lock()->SetText("Level:" + std::to_string(m_vwp_gameLevelManager.lock()->GetGameLevel()));
}

void ButiEngine::UI_GameLevel::OnSet()
{
}

void ButiEngine::UI_GameLevel::OnRemove()
{
}

void ButiEngine::UI_GameLevel::OnShowUI()
{
}

void ButiEngine::UI_GameLevel::Start()
{
	m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	m_vwp_textDraw = gameObject.lock()->GetGameComponent<TextDrawComponent>();
	m_vwp_textDraw.lock()->SetText("Level:" + std::to_string(m_vwp_gameLevelManager.lock()->GetGameLevel()));
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::UI_GameLevel::Clone()
{
	return ObjectFactory::Create<UI_GameLevel>();
}
