#include "stdafx_u.h"
#include "GameTimer.h"

void ButiEngine::GameTimer::OnUpdate()
{
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
	static std::int32_t countSecond = 180;
	if (GUI::DragInt("##CountSecond", countSecond, 1.0f, 0, 600))
	{
		SetCountSecond(countSecond);
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
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(180 * 60);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameTimer::Clone()
{
	return ObjectFactory::Create<GameTimer>();
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
	if (!m_vlp_timer)
	{
		return 0;
	}

	return m_vlp_timer->GetRemainFrame() / 60.0f;
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
