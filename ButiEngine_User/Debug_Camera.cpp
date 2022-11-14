#include "stdafx_u.h"
#include "Debug_Camera.h"

void ButiEngine::Debug_Camera::OnUpdate()
{
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::C))
	{
		m_isDebug = !m_isDebug;
		if (m_isDebug)
		{
			gameObject.lock()->transform->SetLocalPositionZ(15.0f);
		}
		else
		{
			gameObject.lock()->transform->SetLocalPositionZ(5.0f);
		}
	}
}

void ButiEngine::Debug_Camera::OnSet()
{
}

void ButiEngine::Debug_Camera::OnRemove()
{
}

void ButiEngine::Debug_Camera::OnShowUI()
{
}

void ButiEngine::Debug_Camera::Start()
{
#ifdef DEBUG
	m_isDebug = false;
#else
	SetIsRemove(true);
#endif // DEBUG
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Debug_Camera::Clone()
{
	return ObjectFactory::Create<Debug_Camera>();
}
