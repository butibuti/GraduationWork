#include "stdafx_u.h"
#include "FriendCompleteDirecting.h"
#include "PauseManager.h"
#include "GameCamera.h"
#include "FriendBody.h"

void ButiEngine::FriendCompleteDirecting::OnUpdate()
{
	if (m_vlp_directingTimer->Update())
	{
		m_vwp_pauseManager.lock()->SetIsPause(false);
		m_vwp_gameCamera.lock()->ZoomOut(30);
		SetIsRemove(true);
	}
}

void ButiEngine::FriendCompleteDirecting::OnSet()
{
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_gameCamera = GetManager().lock()->GetGameObject("CameraMan").lock()->GetGameComponent<GameCamera>();

	m_vwp_pauseManager.lock()->SetIsPause(true);

	if (gameObject.lock()->GetGameComponent<FriendBody>()->GetScore() == 2)
	{
		m_vwp_gameCamera.lock()->SpecialZoom(30);
	}
	else
	{
		m_vwp_gameCamera.lock()->NormalZoom(30);
	}

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
