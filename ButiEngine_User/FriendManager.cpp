#include "stdafx_u.h"
#include "FriendManager.h"
#include "GameLevelManager.h"

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
	m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();

	m_friendCount = 0;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendManager::Clone()
{
	return ObjectFactory::Create<FriendManager>();
}

void ButiEngine::FriendManager::AddFriendCount()
{
	m_friendCount++;
	m_currentLevelFriendCount++;

	m_vwp_gameLevelManager.lock()->CheckLevelUp(m_currentLevelFriendCount);
}
