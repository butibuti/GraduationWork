#include "stdafx_u.h"
#include "GameTimer.h"
#include "PauseManager.h"
#include "StageManager.h"

#include"Heart.h"
constexpr float div60 = 1.0f / 60;
void ButiEngine::GameTimer::OnUpdate()
{
	m_vwp_heart.lock()->SetScore(GetRemainSecond());
	if (!CanUpdate())
	{
		return;
	}
	if (m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();
	}
}

void ButiEngine::GameTimer::OnSet()
{
}

void ButiEngine::GameTimer::OnRemove()
{
}

void ButiEngine::GameTimer::OnShowUI()
{
	GUI::BulletText(U8("Žc‚èŽžŠÔ"));
	if (GUI::DragInt("##CountSecond", m_countSecond, 1.0f, 0, 600))
	{
		SetCountSecond(m_countSecond);
	}

	if (GUI::Button("Start"))
	{
		StartTimer();
	}

	if (GUI::Button("Stop"))
	{
		StopTimer();
	}

	if (GUI::Button("Reset"))
	{
		ResetTimer();
	}
}

void ButiEngine::GameTimer::Start()
{
	m_vwp_stageManager = GetManager().lock()->GetGameObject("StageManager").lock()->GetGameComponent<StageManager>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(m_countSecond * 60);
	m_vlp_timer->Start();
	m_vwp_heart = GetManager().lock()->GetGameObject("BackHeart").lock()->GetGameComponent<Heart>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameTimer::Clone()
{
	auto clone = ObjectFactory::Create<GameTimer>();
	clone->m_countSecond = m_countSecond;
	return clone;
}

void ButiEngine::GameTimer::SetCountSecond(std::int32_t arg_countSecond)
{
	if (!m_vlp_timer)
	{
		return;
	}

	m_vlp_timer->ChangeCountFrame(arg_countSecond * 60);
}

std::int32_t ButiEngine::GameTimer::GetRemainSecond()
{
	if (!m_vlp_timer || !m_vlp_timer->IsOn())
	{
		return 0;
	}

	if (!m_vwp_stageManager.lock()->IsGameStart())
	{
		return m_countSecond;
	}

	std::int32_t currentSecond = m_vlp_timer->GetCurrentCountFrame() * div60;
	std::int32_t remainSecond = m_countSecond - currentSecond;
	std::int32_t maxSecond = m_vlp_timer->GetMaxCountFrame() * 60.0f;

	return remainSecond;
}

void ButiEngine::GameTimer::StartTimer()
{
	if (!m_vlp_timer)
	{
		return;
	}

	m_vlp_timer->Start();
}

void ButiEngine::GameTimer::StopTimer()
{
	if (!m_vlp_timer)
	{
		return;
	}

	m_vlp_timer->Stop();
}

void ButiEngine::GameTimer::ResetTimer()
{
	if (!m_vlp_timer)
	{
		return;
	}

	m_vlp_timer->Reset();
}

bool ButiEngine::GameTimer::CanUpdate()
{
	if (!m_vwp_stageManager.lock()->IsGameStart())
	{
		return false;
	}

	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return false;
	}

	return true;
}