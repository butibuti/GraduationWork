#include "stdafx_u.h"
#include "TutorialManager.h"
#include "FriendBody.h"
#include "FriendFacePartSpawner.h"
#include "UI_TutorialText.h"
#include "PauseManager.h"

void ButiEngine::TutorialManager::OnUpdate()
{
	if (m_isSceneEnd)
	{
		return;
	}

	if (m_vlp_waitTimer->Update())
	{
		if (m_vwp_pauseManager.lock()->IsPause())
		{
			m_vwp_pauseManager.lock()->SetIsPause(false);
		}

		m_vlp_waitTimer->Stop();
		m_vlp_waitTimer->Reset();

		if (m_tutorialPhase == 1)
		{
			SpawnBody(Vector3(0, -4, 0), 0, 1, 0);
			GetManager().lock()->GetGameObject("FriendFacePartSpawner").lock()->GetGameComponent<FriendFacePartSpawner>()->FirstSpawnFacePart();
		}
		else if (m_tutorialPhase == 2)
		{
			SpawnBody(Vector3(0, -4, 0), 0, 0, 0);
			GetManager().lock()->GetGameObject("FriendFacePartSpawner").lock()->GetGameComponent<FriendFacePartSpawner>()->FirstSpawnFacePart();
		}

		if (m_isClear)
		{
			m_isClear = false;

			std::string tutorialTextName = "UI_TutorialText_" + std::to_string(m_tutorialPhase);
			auto tutorialText = GetManager().lock()->AddObjectFromCereal(tutorialTextName);
			m_vwp_tutorialText = tutorialText.lock()->GetGameComponent<UI_TutorialText>();
		}
	}
}

void ButiEngine::TutorialManager::OnSet()
{
}

void ButiEngine::TutorialManager::OnRemove()
{
}

void ButiEngine::TutorialManager::OnShowUI()
{
}

void ButiEngine::TutorialManager::Start()
{
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_pauseManager.lock()->SetIsPause(true);

	m_tutorialPhase = 0;

	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(180);
	m_vlp_waitTimer->Start();

	m_isClear = true;
	m_isSceneEnd = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TutorialManager::Clone()
{
	return ObjectFactory::Create<TutorialManager>();
}

void ButiEngine::TutorialManager::CheckPhaseClear(const bool arg_isFast, const bool arg_isFront)
{
	if (m_tutorialPhase == 0 || arg_isFast || arg_isFront)
	{
		m_tutorialPhase++;
		if (m_vwp_tutorialText.lock())
		{
			m_vwp_tutorialText.lock()->Clear();
			m_vlp_waitTimer->ChangeCountFrame(120);

			m_isClear = true;
		}
	}

	m_vlp_waitTimer->Start();


	if (m_tutorialPhase == 3)
	{
		GetManager().lock()->AddObjectFromCereal("SceneTransition_FadeIn_GamePlay");
		m_isSceneEnd = true;
	}
}

void ButiEngine::TutorialManager::SpawnBody(const Vector3& arg_spawnPosition, const float arg_moveSpeed, const float arg_rotateSpeed, const float arg_initRotationY)
{
	auto body = GetManager().lock()->AddObjectFromCereal("FriendBody");
	body.lock()->transform->SetLocalPosition(arg_spawnPosition);
	body.lock()->transform->SetLocalRotationY_Degrees(arg_initRotationY);

	auto bodyComponent = body.lock()->GetGameComponent<FriendBody>();
	bodyComponent->SetParameter(arg_moveSpeed, arg_rotateSpeed);
}
