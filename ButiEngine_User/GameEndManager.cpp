#include "stdafx_u.h"
#include "GameEndManager.h"
#include "GameEnd_FriendSpawner.h"
#include "InputManager.h"

void ButiEngine::GameEndManager::OnUpdate()
{
	if (GameDevice::GetInput().GetPadButtonTrigger(ButiInput::PadButtons::XBOX_UP))
	{
		InputManager::SetIsEnableTransitionSceneKey(true);
	}
	else if (GameDevice::GetInput().GetPadButtonTrigger(ButiInput::PadButtons::XBOX_DOWN))
	{
		InputManager::SetIsEnableTransitionSceneKey(false);
	}

	if (InputManager::IsEnableTransitionSceneKey() && GameDevice::GetInput().TriggerKey(ButiInput::Keys::Space))
	{
		m_vlp_timer->Stop();
		GetManager().lock()->AddObjectFromCereal("SceneTransition_FadeIn_GamePlay_1");
		return;
	}

	if (m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();
		if (!InputManager::IsEnableTransitionSceneKey())
		{
			GetManager().lock()->AddObjectFromCereal("SceneTransition_FadeIn_GamePlay_1");
		}
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

	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(600);
	m_vlp_timer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameEndManager::Clone()
{
	return ObjectFactory::Create<GameEndManager>();
}
