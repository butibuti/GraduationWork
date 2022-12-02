#include "stdafx_u.h"
#include "FriendCompleteDirecting.h"
#include "PauseManager.h"
#include "GameCamera.h"
#include "FriendBody.h"
#include "FriendHead.h"
#include "ConcentratedLine.h"

void ButiEngine::FriendCompleteDirecting::OnUpdate()
{
	if (m_vlp_spawnHukidashiIntervalTimer->Update())
	{
		SpawnHukidashi();
	}

	if (m_vlp_waitZoomTimer->Update())
	{
		m_vlp_waitZoomTimer->Stop();

		m_vwp_gameCamera.lock()->StopShake();

		if (m_isSpecialDirecting)
		{
			m_vwp_gameCamera.lock()->SpecialZoom(30);
		}
		else
		{
			m_vwp_gameCamera.lock()->NormalZoom(15);
		}

		auto concentratedLine = GetManager().lock()->AddObjectFromCereal("ConcentratedLine");
		concentratedLine.lock()->GetGameComponent<ConcentratedLine>()->SetLifeTime(60);

		GetManager().lock()->GetGameObject(GameObjectTag("FriendHead")).lock()->GetGameComponent<FriendHead>()->SetIsRemove(true);
	}

	if (m_vlp_directingTimer->Update())
	{
		m_vwp_pauseManager.lock()->SetIsPause(false);
		m_vwp_gameCamera.lock()->ZoomOut(20);
		SetIsRemove(true);
	}
}

void ButiEngine::FriendCompleteDirecting::OnSet()
{
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_cameraMan = GetManager().lock()->GetGameObject("CameraMan");
	m_vwp_gameCamera = m_vwp_cameraMan.lock()->GetGameComponent<GameCamera>();

	m_vwp_pauseManager.lock()->SetIsPause(true);

	std::int32_t shakeFrame = 30;
	m_vwp_gameCamera.lock()->StartShake(shakeFrame);

	m_isSpecialDirecting = false;
	
	auto headComponent = GetManager().lock()->GetGameObject(GameObjectTag("FriendHead")).lock()->GetGameComponent<FriendHead>();
	auto bodyComponent = gameObject.lock()->GetGameComponent<FriendBody>();

	if (headComponent->IsBeautiful() && bodyComponent->IsFast() && bodyComponent->IsFront())
	{
		//m_isSpecialDirecting = true;
	}
	
	if (m_isSpecialDirecting)
	{
		m_vlp_waitZoomTimer = ObjectFactory::Create<RelativeTimer>(shakeFrame - 5);
	}
	else
	{
		m_vlp_waitZoomTimer = ObjectFactory::Create<RelativeTimer>(10);
	}

	m_vlp_waitZoomTimer->Start();

	m_vlp_directingTimer = ObjectFactory::Create<RelativeTimer>(70);
	m_vlp_directingTimer->Start();

	SetHukidashiParameter();

	GetManager().lock()->AddObjectFromCereal("Effect_CompleteFriend");

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

void ButiEngine::FriendCompleteDirecting::SetHukidashiParameter()
{
	auto headComponent = GetManager().lock()->GetGameObject(GameObjectTag("FriendHead")).lock()->GetGameComponent<FriendHead>();
	auto bodyComponent = gameObject.lock()->GetGameComponent<FriendBody>();

	if (m_isSpecialDirecting)
	{
		m_vlp_spawnHukidashiIntervalTimer = ObjectFactory::Create<RelativeTimer>(1);
	}
	else
	{
		m_vlp_spawnHukidashiIntervalTimer = ObjectFactory::Create<RelativeTimer>(1);
		m_vlp_spawnHukidashiIntervalTimer->Start();
	}

	if (headComponent->IsBeautiful())
	{
		m_vec_hukidashiNames.push_back("Effect_Hukidashi_Beautiful");
	}
	if (bodyComponent->IsFast())
	{
		m_vec_hukidashiNames.push_back("Effect_Hukidashi_Fast");
	}
	if (bodyComponent->IsFront())
	{
		m_vec_hukidashiNames.push_back("Effect_Hukidashi_Front");
	}

	SpawnHukidashi();
}
