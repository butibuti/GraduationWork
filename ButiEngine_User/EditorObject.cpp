#include "stdafx_u.h"
#include "EditorObject.h"

void ButiEngine::EditorObject::OnUpdate()
{
}

void ButiEngine::EditorObject::OnSet()
{
}

void ButiEngine::EditorObject::OnRemove()
{
}

void ButiEngine::EditorObject::OnShowUI()
{
}

void ButiEngine::EditorObject::Start()
{
	gameObject.lock()->SetIsRemove(true);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EditorObject::Clone()
{
	return ObjectFactory::Create<EditorObject>();
}
