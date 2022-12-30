#include "stdafx_u.h"
#include "ResultManager.h"

void ButiEngine::ResultManager::OnUpdate()
{
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
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ResultManager::Clone()
{
	return ObjectFactory::Create<ResultManager>();
}
