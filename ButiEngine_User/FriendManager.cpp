#include "stdafx_u.h"
#include "FriendManager.h"
#include "GameLevelManager.h"
#include "Result_CompleteFriend.h"

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
	if (GUI::Button("Spawn"))
	{
		SpawnFriends();
	}
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
	return ObjectFactory::Create<FriendManager>();
}

void ButiEngine::FriendManager::AddFriendCount()
{
	m_friendCount++;
	m_currentLevelFriendCount++;

	m_vwp_gameLevelManager.lock()->CheckLevelUp(m_currentLevelFriendCount);
}

void ButiEngine::FriendManager::SpawnFriends()
{
	Vector3 spawnPos = Vector3(10.0f, 30.0f, 0.0f);

	auto end = g_vec_friendDatas.end();
	for (auto itr = g_vec_friendDatas.begin(); itr != end; ++itr)
	{
		auto completeFriend = GetManager().lock()->AddObjectFromCereal("Result_CompleteFriend");
		completeFriend.lock()->GetGameComponent<Result_CompleteFriend>()->CreateParts((*itr));

		completeFriend.lock()->transform->SetLocalPosition(spawnPos);

		spawnPos.x += -2.0f;
	}
}
