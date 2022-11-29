#include "stdafx_u.h"
#include "StageManager.h"
#include "PauseManager.h"
#include "GameTimer.h"
#include "GameLevelManager.h"

void ButiEngine::StageManager::OnUpdate()
{
	if (!m_isGameStart)
	{
		if (m_vwp_gameLevelManager.lock()->GetGameLevel() == 1)
		{
			StartGame();
		}
		return;
	}

	if (m_vlp_waitPlayBGMTimer->Update())
	{
		m_vlp_waitPlayBGMTimer->Stop();

		auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/BGM.wav"));
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlayBGM(sound, 0.1f);
	}

	if (m_vwp_gameTimer.lock()->GetRemainSecond() == 0)
	{
		m_vwp_pauseManager.lock()->SetIsPause(true);
	}

	if (m_vwp_pauseManager.lock()->IsPause())
	{
		if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::R))
		{
			ResetGame();
		}
		else if (GameDevice::GetInput().GetPadButtonTrigger(ButiInput::PadButtons::XBOX_X))
		{
			ResetGame();
		}
	}
}

void ButiEngine::StageManager::OnSet()
{
}

void ButiEngine::StageManager::OnRemove()
{
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->DestroyBGM();
}

void ButiEngine::StageManager::OnShowUI()
{
}

void ButiEngine::StageManager::Start()
{
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_gameTimer = GetManager().lock()->GetGameObject("GameTimer").lock()->GetGameComponent<GameTimer>();
	m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();

	m_vlp_waitPlayBGMTimer = ObjectFactory::Create<RelativeTimer>(30);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::StageManager::Clone()
{
	return ObjectFactory::Create<StageManager>();
}

void ButiEngine::StageManager::StartGame()
{
	m_isGameStart = true;

	m_vlp_waitPlayBGMTimer->Start();
}

void ButiEngine::StageManager::ResetGame()
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	std::string sceneName = sceneManager->GetCurrentScene()->GetSceneInformation()->GetSceneName();
	sceneManager->RemoveScene(sceneName);
	sceneManager->LoadScene(sceneName);
	sceneManager->ChangeScene(sceneName);
}