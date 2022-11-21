#include "stdafx_u.h"
#include "GameTimer.h"
#include "PauseManager.h"

void ButiEngine::GameTimer::OnUpdate()
{
	if (m_vwp_pauseManager.lock()->IsPause())
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
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(m_countSecond * 60);
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

	return (m_vlp_timer->GetRemainFrame() / 60.0f) + 1;
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
