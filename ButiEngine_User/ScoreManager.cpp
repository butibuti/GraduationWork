#include "stdafx_u.h"
#include "ScoreManager.h"
#include "FriendHead.h"
#include "FriendBody.h"
#include"Heart.h"
void ButiEngine::ScoreManager::OnUpdate()
{
#ifdef DEBUG
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::U)) {
		m_score += 1000;
		m_vlp_heart->SetScore(m_score);
	}
#endif // DEBUG

}

void ButiEngine::ScoreManager::OnSet()
{
}

void ButiEngine::ScoreManager::OnRemove()
{
}

void ButiEngine::ScoreManager::OnShowUI()
{
}

void ButiEngine::ScoreManager::Start()
{
	m_score = 0;
	m_vlp_heart = GetManager().lock()->GetGameObject("BackHeart").lock()->GetGameComponent<Heart>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ScoreManager::Clone()
{
	auto clone = ObjectFactory::Create<ScoreManager>();
	return clone;
}

void ButiEngine::ScoreManager::CalcScore()
{
	auto head = GetManager().lock()->GetGameObject(GameObjectTag("FriendHead"));
	auto body = GetManager().lock()->GetGameObject(GameObjectTag("FriendBody"));

	auto headComponent = head.lock()->GetGameComponent<FriendHead>();
	auto bodyComponent = body.lock()->GetGameComponent<FriendBody>();

	std::int32_t eyeScore = headComponent->GetEyeScore();
	std::int32_t noseScore = headComponent->GetNoseScore();
	std::int32_t mouthScore = headComponent->GetMouthScore();

	std::int32_t headScore = eyeScore + noseScore + mouthScore;
	std::int32_t bodyScore = bodyComponent->GetScore();

	std::int32_t addScore = headScore * bodyScore;

	m_score += addScore;
	m_vlp_heart->SetScore(m_score);
}