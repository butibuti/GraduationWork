#include "stdafx_u.h"
#include "FriendCompleteDirecting.h"
#include "PauseManager.h"
#include "GameCamera.h"
#include "FriendBody.h"
#include "FriendHead.h"
#include "Effect_Belt.h"
#include "Effect_CompleteFriend.h"
#include "BonusFriend.h"
#include "TutorialManager.h"
#include "FriendFacePart.h"
#include "UI_TotalEvaluation.h"
#include "GameLevelManager.h"
#include "FriendBodySpawner.h"

void ButiEngine::FriendCompleteDirecting::OnUpdate()
{
	//if (m_vlp_spawnHukidashiIntervalTimer->Update())
	//{
	//	SpawnHukidashi();
	//}

	if (m_vlp_waitZoomInTimer->Update())
	{
		m_vlp_waitZoomInTimer->Stop();

		if (m_isSpecialDirecting)
		{
			m_vwp_gameCamera.lock()->SpecialZoom(m_zoomFrame);

			m_vwp_belt_left = GetManager().lock()->AddObjectFromCereal("Effect_Belt");
			m_vwp_belt_left.lock()->GetGameComponent<Effect_Belt>()->SetIsSpawnToRight(false);
			m_vwp_belt_right = GetManager().lock()->AddObjectFromCereal("Effect_Belt");
			m_vwp_belt_right.lock()->GetGameComponent<Effect_Belt>()->SetIsSpawnToRight(true);
		}
		else
		{
			m_vwp_gameCamera.lock()->NormalZoom(m_zoomFrame);
		}

		//m_vlp_spawnHukidashiIntervalTimer->Start();
		//SpawnHukidashi();

		if (!m_isNoAppearEvaluation)
		{
			GetManager().lock()->GetGameObject("UI_TotalEvaluation").lock()->GetGameComponent<UI_TotalEvaluation>()->Appear();
		}

		auto concentratedLine = GetManager().lock()->AddObjectFromCereal("Effect_ConcentratedLine");

		GetManager().lock()->GetGameObject(GameObjectTag("FriendHead")).lock()->GetGameComponent<FriendHead>()->SetIsRemove(true);
	}

	if (m_vlp_directingTimer->Update())
	{
		if (m_isSpecialDirecting)
		{
			m_vwp_belt_left.lock()->GetGameComponent<Effect_Belt>()->Disappear(8);
			m_vwp_belt_right.lock()->GetGameComponent<Effect_Belt>()->Disappear(8);
		}

		if (!m_isNoAppearEvaluation)
		{
			GetManager().lock()->GetGameObject("UI_TotalEvaluation").lock()->GetGameComponent<UI_TotalEvaluation>()->Disappear();
		}

		m_vwp_pauseManager.lock()->SetIsPause(false);
		m_vwp_gameCamera.lock()->ZoomOut(20);

		auto bodyComponent = gameObject.lock()->GetGameComponent<FriendBody>();
		auto bonusFriends = bodyComponent->GetBonusFriens();
		auto end = bonusFriends.end();
		for (auto itr = bonusFriends.begin(); itr != end; ++itr)
		{
			(*itr).lock()->GetGameComponent<BonusFriend>()->StartMoveBack();
		}

		auto tutorialManager = GetManager().lock()->GetGameObject("TutorialManager");
		if (tutorialManager.lock())
		{
			if (tutorialManager.lock()->GetGameComponent<TutorialManager>()->GetTutorialPhase() != 3)
			{
				auto head = GetManager().lock()->AddObjectFromCereal("FriendHead");
				head.lock()->transform->SetLocalPosition(Vector3(0.0f, -10.0f, 0.0f));
			}
		}
		else
		{
			auto friendBodySpawner = GetManager().lock()->GetGameObject("FriendBodySpawner").lock()->GetGameComponent<FriendBodySpawner>();
			if (friendBodySpawner->GetBodiesNumber() > 0)
			{
				auto head = GetManager().lock()->AddObjectFromCereal("FriendHead");
				head.lock()->transform->SetLocalPosition(Vector3(0.0f, -10.0f, 0.0f));
			}
		}

		SetIsRemove(true);
	}
}

void ButiEngine::FriendCompleteDirecting::OnSet()
{
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_cameraMan = GetManager().lock()->GetGameObject("CameraParent");
	m_vwp_gameCamera = m_vwp_cameraMan.lock()->GetGameComponent<GameCamera>();

	m_vwp_pauseManager.lock()->SetIsPause(true);

	m_isSpecialDirecting = false;
	m_isNoAppearEvaluation = false;
	
	auto headComponent = GetManager().lock()->GetGameObject(GameObjectTag("FriendHead")).lock()->GetGameComponent<FriendHead>();
	auto bodyComponent = gameObject.lock()->GetGameComponent<FriendBody>();

	auto gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	std::int32_t gameLevel = gameLevelManager->GetGameLevel();

	if (gameLevel != 0 && bodyComponent->GetTotalRank() == Rank::Good)
	{
		m_isSpecialDirecting = true;
	}

	if (gameLevel == 0)
	{
		m_isNoAppearEvaluation = true;
	}

	Rank eyeRank = headComponent->GetEye().lock()->GetGameComponent<FriendFacePart>()->GetPartRank();
	Rank noseRank = headComponent->GetNose().lock()->GetGameComponent<FriendFacePart>()->GetPartRank();
	Rank mouthRank = headComponent->GetMouth().lock()->GetGameComponent<FriendFacePart>()->GetPartRank();

	auto totalEvaluation = GetManager().lock()->GetGameObject("UI_TotalEvaluation").lock()->GetGameComponent<UI_TotalEvaluation>();
	totalEvaluation->SetTotalRank(bodyComponent->GetTotalRank());
	totalEvaluation->SetPartRank(eyeRank, noseRank, mouthRank);

	SetGameCameraParameter();
	//SetHukidashiParameter();
	AppearBonusFriends();

	m_vlp_directingTimer = ObjectFactory::Create<RelativeTimer>(m_waitZoomInFrame + m_waitZoomOutFrame);
	m_vlp_directingTimer->Start();

	auto completeFriendEffect = GetManager().lock()->AddObjectFromCereal("Effect_CompleteFriend");
	auto neck = bodyComponent->GetNeck();
	completeFriendEffect.lock()->transform->SetLocalPosition(neck.lock()->transform->GetWorldPosition());

	auto effectComponent = completeFriendEffect.lock()->GetGameComponent<Effect_CompleteFriend>();
	effectComponent->SetBody(gameObject);

	auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/CompleteFriend.wav"));
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);
}

void ButiEngine::FriendCompleteDirecting::OnRemove()
{
}

void ButiEngine::FriendCompleteDirecting::OnShowUI()
{
}

void ButiEngine::FriendCompleteDirecting::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendCompleteDirecting::Clone()
{
	return ObjectFactory::Create<FriendCompleteDirecting>();
}

void ButiEngine::FriendCompleteDirecting::SpawnHukidashi()
{
	if (m_vec_hukidashiNames.size() == 0)
	{
		return;
	}

	GetManager().lock()->AddObjectFromCereal(m_vec_hukidashiNames[0]);

	m_vec_hukidashiNames.erase(m_vec_hukidashiNames.begin());

	if (m_vec_hukidashiNames.size() == 0)
	{
		m_vlp_spawnHukidashiIntervalTimer->Stop();
	}
}

void ButiEngine::FriendCompleteDirecting::SetGameCameraParameter()
{
	//auto headCenter = GetManager().lock()->GetGameObject(GameObjectTag("HeadCenter"));
	//Vector3 lookTargetPos = headCenter.lock()->transform->GetWorldPosition();
	//m_vwp_gameCamera.lock()->StartLookAtTarget(lookTargetPos, 5);

	std::int32_t shakeFrame = 15;
	m_waitZoomInFrame = 10;
	m_waitZoomOutFrame = 30;
	m_zoomFrame = 10;

	if (m_isSpecialDirecting)
	{
		shakeFrame = 20;
		m_waitZoomInFrame = 30;
		m_waitZoomOutFrame = 60;
		m_zoomFrame = 8;
	}

	m_vwp_gameCamera.lock()->StartShake(shakeFrame);

	m_vlp_waitZoomInTimer = ObjectFactory::Create<RelativeTimer>(m_waitZoomInFrame);
	m_vlp_waitZoomInTimer->Start();

	m_waitZoomOutFrame += m_zoomFrame;
}

void ButiEngine::FriendCompleteDirecting::SetHukidashiParameter()
{
	auto headComponent = GetManager().lock()->GetGameObject(GameObjectTag("FriendHead")).lock()->GetGameComponent<FriendHead>();
	auto bodyComponent = gameObject.lock()->GetGameComponent<FriendBody>();

	if (m_isSpecialDirecting)
	{
		m_vlp_spawnHukidashiIntervalTimer = ObjectFactory::Create<RelativeTimer>(10);
	}
	else
	{
		m_vlp_spawnHukidashiIntervalTimer = ObjectFactory::Create<RelativeTimer>(10);
	}
	
	//if (headComponent->IsBeautiful())
	//{
	//	m_vec_hukidashiNames.push_back("Effect_Hukidashi_Beautiful");
	//}
	
	if (headComponent->IsGood())
	{
		m_vec_hukidashiNames.push_back("Effect_Hukidashi_Fast");
	}

	//if (headComponent->IsFast())
	//{
	//	m_vec_hukidashiNames.push_back("Effect_Hukidashi_Fast");
	//}
	if (bodyComponent->IsFront())
	{
		m_vec_hukidashiNames.push_back("Effect_Hukidashi_Front");
	}
}

void ButiEngine::FriendCompleteDirecting::AppearBonusFriends()
{
	auto bodyComponent = gameObject.lock()->GetGameComponent<FriendBody>();
	auto bonusFriends = bodyComponent->GetBonusFriens();
	auto end = bonusFriends.end();
	std::int32_t index = 0;
	for (auto itr = bonusFriends.begin(); itr != end; ++itr)
	{
		(*itr).lock()->GetGameComponent<BonusFriend>()->Appear(index);
		index++;
	}
}
