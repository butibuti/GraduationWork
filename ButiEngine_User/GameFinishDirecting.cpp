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
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->DestroyBGM();

	GetManager().lock()->AddObjectFromCereal("Text_GameFinish");
	auto belt_left = GetManager().lock()->AddObjectFromCereal("Effect_Belt");
	belt_left.lock()->GetGameComponent<Effect_Belt>()->SetIsSpawnToRight(false);
	auto belt_right = GetManager().lock()->AddObjectFromCereal("Effect_Belt");
	belt_right.lock()->GetGameComponent<Effect_Belt>()->SetIsSpawnToRight(true);

	GetManager().lock()->GetGameObject("CameraParent").lock()->GetGameComponent<GameCamera>()->GameFinishZoom();
	GetManager().lock()->AddObjectFromCereal("SceneTransition_FadeIn_Result");

	auto exInfo = GetManager().lock()->GetGameObject("MainScreen").lock()->GetGameComponent<MeshDrawComponent>()->GetExInfo();
	exInfo.x = 0.0f;
	GetManager().lock()->GetGameObject("MainScreen").lock()->GetGameComponent<MeshDrawComponent>()->SetExInfo(exInfo);

	auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/GameFinish.wav"));
	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);
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
