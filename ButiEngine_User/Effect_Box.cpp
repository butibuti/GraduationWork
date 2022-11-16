#include "stdafx_u.h"
#include "Effect_Box.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

void ButiEngine::Effect_Box::OnUpdate()
{
}

void ButiEngine::Effect_Box::OnSet()
{
}

void ButiEngine::Effect_Box::OnRemove()
{
}

void ButiEngine::Effect_Box::OnShowUI()
{
}

void ButiEngine::Effect_Box::Start()
{
	//gameObject.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->SetGravity(-Vector3Const::ZAxis * 9.8f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Effect_Box::Clone()
{
	return ObjectFactory::Create<Effect_Box>();
}
