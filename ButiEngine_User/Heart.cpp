#include "stdafx_u.h"
#include "Heart.h"
#include"ScoreNumber.h"
#include"GameTimer.h"
std::int32_t pows[]{1,10,100,1000,10000 ,100000,1000000,10000000,100000000,1000000000 };

void ButiEngine::Heart::OnUpdate()
{
}

void ButiEngine::Heart::OnSet()
{
}

void ButiEngine::Heart::Start()
{
	m_maxScore = GetManager().lock()->GetGameObject("GameTimer").lock()->GetGameComponent<GameTimer>()->GetCountSecond();// pows[m_maxDigit] - 1;
	float xOffset = (m_maxDigit-1) * -0.5f;
	for (std::int32_t index = 0; index < m_maxDigit; index++) {
		auto numObject = GetManager().lock()->AddObjectFromCereal("Number");
		
		numObject.lock()->transform->SetLocalPositionX((xOffset + index) * m_letterSpace);
		m_list_vlp_scoreNumbers.Add(numObject.lock()->GetGameComponent<ScoreNumber>());
	}
}

void ButiEngine::Heart::OnRemove()
{
}

void ButiEngine::Heart::OnShowUI()
{
	GUI::DragInt("Digit", m_maxDigit, 1, 1, 12);
	GUI::DragFloat("MaxHeartSize", m_maxHeartSize, 0.01f, 0.0f, 100.0f);
	GUI::DragFloat("MinHeartSize", m_minHeartSize, 0.01f, 0.0f, 0.01f);
	GUI::DragFloat("RotationSpeed", m_rotationSpeed, 0.01f, 0.0f, 10.0f);
	GUI::DragFloat("LetterSpace", m_letterSpace, 0.01f, 0.0f, 10.0f);
}

void ButiEngine::Heart::SetScore(const std::int32_t arg_score)
{
	if (m_currentScore == arg_score) {
		return;
	}
	m_currentScore = arg_score;
	m_displayingScore= m_currentScore < m_maxScore ? arg_score : m_maxScore;

	for (std::int32_t index = 0; index < m_maxDigit; index++) {
		auto n= m_displayingScore%pows[index+1];
		n /= pows[index];
		m_list_vlp_scoreNumbers[index]->SetNumber(static_cast<std::int8_t>( n));
	}
	float size = m_minHeartSize + (m_maxHeartSize - m_minHeartSize) * (static_cast<float>( m_displayingScore)/ static_cast<float>(m_maxScore));
	auto scale =gameObject.lock()->transform->GetLocalScale();
	scale.x = size; 
	scale.y = size;
	gameObject.lock()->transform->SetLocalScale(scale);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Heart::Clone()
{
	auto output = ObjectFactory::Create<Heart>();
	output->m_rotationSpeed = m_rotationSpeed;
	output->m_maxHeartSize =m_maxHeartSize;
	output->m_minHeartSize= m_minHeartSize;
	output->m_maxDigit = m_maxDigit;
    return output;
}
