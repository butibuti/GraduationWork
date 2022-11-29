#include "stdafx_u.h"
#include "FriendCompleteDirecting.h"
#include "PauseManager.h"
#include "GameCamera.h"
#include "FriendBody.h"
#include "FriendHead.h"
#include "ConcentratedLine.h"

void ButiEngine::FriendCompleteDirecting::OnUpdate()
{
	if (m_vlp_waitZoomTimer->Update())
	{
		m_vlp_waitZoomTimer->Stop();

		if (gameObject.lock()->GetGameComponent<FriendBody>()->GetScore() == 2)
		{
			m_vwp_gameCamera.lock()->SpecialZoom(30);
		}
		else
		{
			m_vwp_gameCamera.lock()->NormalZoom(30);
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

	std::int32_t shakeFrame = 20;
	m_vwp_gameCamera.lock()->Shake(shakeFrame);
	
	m_vlp_waitZoomTimer = ObjectFactory::Create<RelativeTimer>(shakeFrame - 5);
	m_vlp_waitZoomTimer->Start();

	m_vlp_directingTimer = ObjectFactory::Create<RelativeTimer>(60);
	m_vlp_directingTimer->Start();
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
