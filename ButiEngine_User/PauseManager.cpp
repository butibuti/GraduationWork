#include "stdafx_u.h"
#include "PauseManager.h"
#include "ButiBulletWrap/ButiBulletWrap/PhysicsManager.h"
#include "ButiBulletWrap/ButiBulletWrap/PhysicsWorld.h"

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
	GUI::Checkbox("isPause", m_isPause);
}

void ButiEngine::PauseManager::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::PauseManager::Clone()
{
	return ObjectFactory::Create<PauseManager>();
}

void ButiEngine::PauseManager::SetIsPause(const bool arg_isPause)
{
	m_isPause = arg_isPause;
	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetPhysicsManager()->GetActivePhysicsWorld()->SetIsPause(arg_isPause);
}
