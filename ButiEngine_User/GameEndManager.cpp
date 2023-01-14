#include "stdafx_u.h"
#include "GameEndManager.h"
#include "GameEnd_FriendSpawner.h"

void ButiEngine::GameEndManager::OnUpdate()
{
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::Space))
	{
		GetManager().lock()->AddObjectFromCereal("SceneTransition_FadeIn_GamePlay_1");
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
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameEndManager::Clone()
{
	return ObjectFactory::Create<GameEndManager>();
}
