#include "stdafx_u.h"
#include "FriendManager.h"
#include "GameLevelManager.h"
#include "UI_FriendCount.h"

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

void ButiEngine::FriendManager::AddCompleteFriend(Value_weak_ptr<GameObject> arg_friend, Value_ptr<FriendData> arg_data)
{
	g_vec_completeFriends.push_back(arg_friend);
	g_vec_friendDatas.push_back(arg_data);

	GetManager().lock()->GetGameObject("UI_FriendCount").lock()->GetGameComponent<UI_FriendCount>()->AddCount();
}

void ButiEngine::FriendManager::RemoveCompleteFriend(const std::int32_t arg_index)
{
	g_vec_completeFriends.erase(g_vec_completeFriends.begin() + arg_index);
	g_vec_friendDatas.erase(g_vec_friendDatas.begin() + arg_index);

	GetManager().lock()->GetGameObject("UI_FriendCount").lock()->GetGameComponent<UI_FriendCount>()->RemoveCount();
}
