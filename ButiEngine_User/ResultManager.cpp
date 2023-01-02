#include "stdafx_u.h"
#include "ResultManager.h"
#include "FriendManager.h"
#include "Result_Camera.h"
#include "Result_BackHuman.h"
#include "Result_FriendFallPoint.h"

void ButiEngine::ResultManager::OnUpdate()
{
	CheckStartZoomIn();
	CheckStartZoomOut();
}

void ButiEngine::ResultManager::OnSet()
{
}

void ButiEngine::ResultManager::OnRemove()
{
}

void ButiEngine::ResultManager::OnShowUI()
{
}

void ButiEngine::ResultManager::Start()
{
	m_vwp_friendManager = GetManager().lock()->GetGameObject("FriendManager").lock()->GetGameComponent<FriendManager>();
	m_vwp_camera = GetManager().lock()->GetGameObject("CameraMan").lock()->GetGameComponent<Result_Camera>();
	m_vwp_backHuman = GetManager().lock()->GetGameObject("BackHuman_Body").lock()->GetGameComponent<Result_BackHuman>();
	m_vwp_fallPoint = GetManager().lock()->GetGameObject("FallPoint").lock()->GetGameComponent<Result_FriendFallPoint>();

	m_isStartedZoomIn = false;
	m_isStartedZoomOut = false;

	m_vwp_fallPoint.lock()->StartMove();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ResultManager::Clone()
{
	return ObjectFactory::Create<ResultManager>();
}

void ButiEngine::ResultManager::CheckStartZoomIn()
{
	if (!m_vwp_fallPoint.lock() || m_isStartedZoomIn)
	{
		return;
	}

	if (m_vwp_fallPoint.lock()->IsStartZoomIn())
	{
		m_vwp_camera.lock()->ZoomIn();

		m_vwp_backHuman.lock()->StartTurnAnimation();

		m_isStartedZoomIn = true;
	}
}

void ButiEngine::ResultManager::CheckStartZoomOut()
{
	if (!m_vwp_fallPoint.lock() || m_isStartedZoomOut)
	{
		return;
	}

	if (m_vwp_fallPoint.lock()->IsStartZoomOut())
	{
		m_vwp_camera.lock()->ZoomOut();

		if (m_successBorder >= m_vwp_friendManager.lock()->GetFriendCount())
		{
			m_vwp_backHuman.lock()->StartTurnSuccessAnimation();
			GetManager().lock()->AddObjectFromCereal("Effect_ConcentratedLine_RedNYellow");
		}
		else
		{
			m_vwp_backHuman.lock()->StartTurnFailedAnimation();
		}

		m_isStartedZoomOut = true;
	}
}
