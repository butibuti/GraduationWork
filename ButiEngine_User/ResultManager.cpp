#include "stdafx_u.h"
#include "ResultManager.h"
#include "Result_Camera.h"
#include "Result_BackHuman.h"
#include "Result_FriendFallPoint.h"
#include "Result_FriendSpawner.h"

void ButiEngine::ResultManager::OnUpdate()
{
	if (m_vlp_waitStartFallTimer && m_vlp_waitStartFallTimer->Update())
	{
		m_vlp_waitStartFallTimer->Stop();
		m_vwp_fallPoint.lock()->StartMove();
	}
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
	GUI::BulletText("SuccessBorder");
	GUI::DragInt("##SuccessBorder", m_successBorder, 1.0f, 0, 100);
}

void ButiEngine::ResultManager::Start()
{
	m_vwp_camera = GetManager().lock()->GetGameObject("CameraMan").lock()->GetGameComponent<Result_Camera>();
	m_vwp_backHuman = GetManager().lock()->GetGameObject("BackHuman_Body").lock()->GetGameComponent<Result_BackHuman>();
	m_vwp_fallPoint = GetManager().lock()->GetGameObject("FallPoint").lock()->GetGameComponent<Result_FriendFallPoint>();

	m_isStartedZoomIn = false;
	m_isStartedZoomOut = false;

#ifdef DEBUG
	return;
#endif // DEBUG

	GetManager().lock()->GetGameObject("Result_FriendSpawner").lock()->GetGameComponent<Result_FriendSpawner>()->SpawnFriends();

	m_vlp_waitStartFallTimer = ObjectFactory::Create<RelativeTimer>(60);
	m_vlp_waitStartFallTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ResultManager::Clone()
{
	auto clone = ObjectFactory::Create<ResultManager>();
	clone->m_successBorder = m_successBorder;
	return clone;
}

void ButiEngine::ResultManager::StartFailedTimer()
{
	std::int32_t friendCount = GetManager().lock()->GetGameObjects(GameObjectTag("CompleteFriend")).GetSize();
	if (m_successBorder <= friendCount)
	{
		return;
	}

	m_isStartedZoomIn = true;
	m_isStartedZoomOut = true;
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

		std::int32_t friendCount = GetManager().lock()->GetGameObjects(GameObjectTag("CompleteFriend")).GetSize();
		if (m_successBorder <= friendCount)
		{
			StartSuccess();
		}
		else
		{
			StartFailed();
		}

		m_isStartedZoomOut = true;
	}
}

void ButiEngine::ResultManager::StartSuccess()
{
	m_vwp_backHuman.lock()->StartTurnSuccessAnimation();
	GetManager().lock()->AddObjectFromCereal("Effect_ConcentratedLine_RedNYellow");
	GetManager().lock()->AddObjectFromCereal("Text_Success");
	GetManager().lock()->AddObjectFromCereal("SceneTransition_FadeIn_GameEnd");

	auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/Success.wav"));
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);

	sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/Success2.wav"));
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);
}

void ButiEngine::ResultManager::StartFailed()
{
	m_vwp_backHuman.lock()->StartTurnFailedAnimation();
	GetManager().lock()->AddObjectFromCereal("Background_Gray");
	GetManager().lock()->AddObjectFromCereal("Text_Failed");
	GetManager().lock()->AddObjectFromCereal("SceneTransition_FadeIn_GameEnd");

	auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/Failed.wav"));
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);
}
