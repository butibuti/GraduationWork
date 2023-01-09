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
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ScoreManager::Clone()
{
	auto clone = ObjectFactory::Create<ScoreManager>();
	return clone;
}

void ButiEngine::ScoreManager::CalcScore(Value_weak_ptr<GameObject> arg_vwp_head, Value_weak_ptr<GameObject> arg_vwp_body)
{
	auto headComponent = arg_vwp_head.lock()->GetGameComponent<FriendHead>();
	auto bodyComponent = arg_vwp_body.lock()->GetGameComponent<FriendBody>();

	std::int32_t addScore = 0;
	addScore += headComponent->GetEyeScore();
	addScore += headComponent->GetNoseScore();
	addScore += headComponent->GetMouthScore();

	addScore *= 2 * (1 + headComponent->IsBeautiful());
	addScore *= 2 * (1 + bodyComponent->IsFront());
	addScore *= 2 * (1 + bodyComponent->IsFast());

	m_score += addScore;
	//m_vlp_heart->SetScore(m_score);
}