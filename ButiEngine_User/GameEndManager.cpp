#include "stdafx_u.h"
#include "GameEndManager.h"
#include "GameEnd_FriendSpawner.h"

void ButiEngine::GameEndManager::OnUpdate()
{
	if (m_vlp_gameEndTimer->Update())
	{
		GetManager().lock()->AddObjectFromCereal("SceneTransition_FadeIn_GameEnd");
	}
}

void ButiEngine::GameEndManager::OnSet()
{
}

void ButiEngine::GameEndManager::OnRemove()
{
}

void ButiEngine::GameEndManager::OnShowUI()
{
}

void ButiEngine::GameEndManager::Start()
{
	auto friendSpawner = GetManager().lock()->GetGameObject("FriendSpawner").lock()->GetGameComponent<GameEnd_FriendSpawner>();
	friendSpawner->SpawnFriends();

	m_vlp_gameEndTimer = ObjectFactory::Create<RelativeTimer>(600);
	m_vlp_gameEndTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameEndManager::Clone()
{
	return ObjectFactory::Create<GameEndManager>();
}
