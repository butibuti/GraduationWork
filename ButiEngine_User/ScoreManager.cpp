#include "stdafx_u.h"
#include "ScoreManager.h"

void ButiEngine::ScoreManager::OnUpdate()
{
}

void ButiEngine::ScoreManager::OnSet()
{
}

void ButiEngine::ScoreManager::OnRemove()
{
}

void ButiEngine::ScoreManager::OnShowUI()
{
	GUI::Text("Score:" + std::to_string(m_score));
	GUI::BulletText(U8("ÉXÉRÉAè„å¿"));
	GUI::DragInt("##MaxScore", m_maxScore, 1.0f, 0, 10000);
}

void ButiEngine::ScoreManager::Start()
{
	m_score = 0;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ScoreManager::Clone()
{
	auto clone = ObjectFactory::Create<ScoreManager>();
	clone->m_maxScore = m_maxScore;
	return clone;
}

void ButiEngine::ScoreManager::AddScore(const std::int32_t arg_addScore)
{
	m_score += arg_addScore;
	m_score = min(m_score, m_maxScore);
}

void ButiEngine::ScoreManager::RemoveScore(const std::int32_t arg_removeScore)
{
	m_score -= arg_removeScore;
	m_score = max(m_score, 0);
}
