#include "stdafx_u.h"
#include "PauseManager.h"

void ButiEngine::PauseManager::OnUpdate()
{
}

void ButiEngine::PauseManager::OnSet()
{
}

void ButiEngine::PauseManager::OnRemove()
{
}

void ButiEngine::PauseManager::OnShowUI()
{
}

void ButiEngine::PauseManager::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::PauseManager::Clone()
{
	return ObjectFactory::Create<PauseManager>();
}
