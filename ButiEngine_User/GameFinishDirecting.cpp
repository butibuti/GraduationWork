#include "stdafx_u.h"
#include "GameFinishDirecting.h"
#include "Effect_Belt.h"
#include "GameCamera.h"
#include "SceneTransition_Fade.h"

void ButiEngine::GameFinishDirecting::OnUpdate()
{
}

void ButiEngine::GameFinishDirecting::OnSet()
{
	GetManager().lock()->AddObjectFromCereal("Text_GameFinish");
	auto belt_left = GetManager().lock()->AddObjectFromCereal("Effect_Belt");
	belt_left.lock()->GetGameComponent<Effect_Belt>()->SetIsSpawnToRight(false);
	auto belt_right = GetManager().lock()->AddObjectFromCereal("Effect_Belt");
	belt_right.lock()->GetGameComponent<Effect_Belt>()->SetIsSpawnToRight(true);

	GetManager().lock()->GetGameObject("CameraParent").lock()->GetGameComponent<GameCamera>()->GameFinishZoom();
	GetManager().lock()->AddObjectFromCereal("SceneTransition_FadeIn_GamePlay");
}

void ButiEngine::GameFinishDirecting::OnRemove()
{
}

void ButiEngine::GameFinishDirecting::OnShowUI()
{
}

void ButiEngine::GameFinishDirecting::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameFinishDirecting::Clone()
{
	return ObjectFactory::Create<GameFinishDirecting>();
}
