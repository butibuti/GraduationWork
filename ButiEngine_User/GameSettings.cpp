#include "stdafx_u.h"
#include "GameSettings.h"

void ButiEngine::GameSettings::OnUpdate()
{
}

void ButiEngine::GameSettings::OnSet()
{
}

void ButiEngine::GameSettings::OnRemove()
{
}

void ButiEngine::GameSettings::OnShowUI()
{
}

void ButiEngine::GameSettings::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameSettings::Clone()
{
	auto clone = ObjectFactory::Create<GameSettings>();
	clone->m_monitorRightTopPos = m_monitorRightTopPos;
	clone->m_monitorLeftBottomPos = m_monitorLeftBottomPos;
	clone->m_tablePos = m_tablePos;
	return clone;
}
