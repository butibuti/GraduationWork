#include "stdafx_u.h"
#include "FriendManager.h"
#include "GameLevelManager.h"
#include "Result_CompleteFriend.h"

#ifdef DEBUG
#include "Header/GameObjects/DefaultGameComponent/TriggerComponent.h"
#include "FriendHead_PartHitArea.h"
#endif // DEBUG

std::vector<ButiEngine::Value_weak_ptr<ButiEngine::GameObject>> ButiEngine::FriendManager::g_vec_completeFriends;
std::vector<ButiEngine::Value_ptr<ButiEngine::FriendData>> ButiEngine::FriendManager::g_vec_friendDatas;

void ButiEngine::FriendManager::OnUpdate()
{
}

void ButiEngine::FriendManager::OnSet()
{
}

void ButiEngine::FriendManager::OnRemove()
{
}

void ButiEngine::FriendManager::OnShowUI()
{
}

void ButiEngine::FriendManager::Start()
{
	auto gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager");
	if (gameLevelManager.lock())
	{
		m_vwp_gameLevelManager = gameLevelManager.lock()->GetGameComponent<GameLevelManager>();
	}

	m_friendCount = 0;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendManager::Clone()
{
	auto clone = ObjectFactory::Create<FriendManager>();
	return clone;
}

void ButiEngine::FriendManager::AddFriendCount()
{
	m_friendCount++;
	m_currentLevelFriendCount++;

	m_vwp_gameLevelManager.lock()->CheckLevelUp(m_currentLevelFriendCount);
}
