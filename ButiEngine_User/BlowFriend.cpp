#include "stdafx_u.h"
#include "BlowFriend.h"

void ButiEngine::BlowFriend::OnUpdate()
{
	Vector3 pos = gameObject.lock()->transform->GetWorldPosition();

}

void ButiEngine::BlowFriend::OnSet()
{
}

void ButiEngine::BlowFriend::OnRemove()
{
}

void ButiEngine::BlowFriend::OnShowUI()
{
}

void ButiEngine::BlowFriend::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BlowFriend::Clone()
{
	return ObjectFactory::Create<BlowFriend>();
}
