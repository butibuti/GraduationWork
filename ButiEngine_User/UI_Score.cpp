#include "stdafx_u.h"
#include "UI_Score.h"
#include "ScoreManager.h"

void ButiEngine::UI_Score::OnUpdate()
{
	m_vwp_textDraw.lock()->SetText("Score:" + std::to_string(m_vwp_scoreManager.lock()->GetScore()));
}

void ButiEngine::UI_Score::OnSet()
{
}

void ButiEngine::UI_Score::OnRemove()
{
}

void ButiEngine::UI_Score::OnShowUI()
{
}

void ButiEngine::UI_Score::Start()
{
	m_vwp_scoreManager = GetManager().lock()->GetGameObject("ScoreManager").lock()->GetGameComponent<ScoreManager>();
	m_vwp_textDraw = gameObject.lock()->GetGameComponent<TextDrawComponent>();
	m_vwp_textDraw.lock()->SetText("Score:" + std::to_string(m_vwp_scoreManager.lock()->GetScore()));
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::UI_Score::Clone()
{
	return ObjectFactory::Create<UI_Score>();
}
