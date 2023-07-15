#include "stdafx_u.h"
#include "CompleteFriend.h"
#include "FriendManager.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include "FriendFacePart.h"
#include "PauseManager.h"
#include "BlowFriend.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "BrightnessController.h"
#include "SeparateDrawObject.h"

void ButiEngine::CompleteFriend::OnUpdate()
{
	if (!m_vwp_pauseManager.lock())
	{
		m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
		return;
	}

	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return;
	}

	if (m_isDance || m_isBlow)
	{
		m_vlp_animationController->Update();
	}
}

void ButiEngine::CompleteFriend::OnSet()
{
	gameObject.lock()->SetGameObjectTag(GameObjectTag("CompleteFriend"));
	m_isDance = false;
}

void ButiEngine::CompleteFriend::OnRemove()
{
}

void ButiEngine::CompleteFriend::OnShowUI()
{
	if (GUI::Button("Blow"))
	{
		gameObject.lock()->AddGameComponent<BlowFriend>();
	}
}

void ButiEngine::CompleteFriend::Start()
{
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::CompleteFriend::Clone()
{
	return ObjectFactory::Create<CompleteFriend>();
}

void ButiEngine::CompleteFriend::SetBody(Value_weak_ptr<GameObject> arg_body)
{
	m_vwp_body = arg_body;

	auto modelDraw = m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>();
	m_vlp_animationController = ButiRendering::CreateAnimationController(modelDraw->GetBone());
}

void ButiEngine::CompleteFriend::Dead(bool arg_isRemoveCompleteFirnedList, bool arg_isAddTargetPosIndex)
{
	m_vwp_head.lock()->SetIsRemove(true);
	m_vwp_body.lock()->SetIsRemove(true);
	m_vwp_heart.lock()->SetIsRemove(true);
	m_vwp_eye.lock()->SetIsRemove(true);
	m_vwp_nose.lock()->SetIsRemove(true);
	m_vwp_mouth.lock()->SetIsRemove(true);
	if (m_vwp_helmet.lock())
	{
		m_vwp_helmet.lock()->SetIsRemove(true);
	}
	if (m_vwp_chara.lock())
	{
		m_vwp_chara.lock()->SetIsRemove(true);
	}
	gameObject.lock()->SetIsRemove(true);

	if (arg_isRemoveCompleteFirnedList)
	{
		GetManager().lock()->GetGameObject("FriendManager").lock()->GetGameComponent<FriendManager>()->RemoveCompleteFriend(gameObject);
	}

	if (arg_isAddTargetPosIndex)
	{
		FriendManager::AddCompleteFriendTargetPositionIndex(m_targetPosIndex);
	}
}

void ButiEngine::CompleteFriend::CreateParts(Value_weak_ptr<FriendData> arg_vwp_friendData)
{
	SetFriendData(arg_vwp_friendData);

	CreateBody(arg_vwp_friendData.lock()->vlp_bodyTransform);
	CreateHead(arg_vwp_friendData.lock()->vlp_headTransform);
	CreateEye(arg_vwp_friendData.lock()->vlp_eyeTransform, arg_vwp_friendData.lock()->eyeRank);
	CreateNose(arg_vwp_friendData.lock()->vlp_noseTransform, arg_vwp_friendData.lock()->noseRank);
	CreateMouth(arg_vwp_friendData.lock()->vlp_mouthTransform, arg_vwp_friendData.lock()->mouthRank);
	if (arg_vwp_friendData.lock()->hasHelmet)
	{
		CreateHelmet();
	}
	if (arg_vwp_friendData.lock()->totalRank == Rank::Good)
	{
		CreateChara();
	}
}

void ButiEngine::CompleteFriend::StartDance()
{
	m_isDance = true;

	std::int32_t animIndex = 4;
	if (m_vwp_friendData.lock()->totalRank == Rank::Bad)
	{
		animIndex = 7;
	}
	else if (m_vwp_friendData.lock()->totalRank == Rank::Normal)
	{
		animIndex = 5;
	}
	else if (m_vwp_friendData.lock()->totalRank == Rank::Good)
	{
		animIndex = 6;
	}

	m_vlp_animationController->ChangeAnimation(60.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[animIndex]->GetAnimation());

	m_vlp_animationController->GetCurrentModelAnimation()->SetIsLoop(true);
}

void ButiEngine::CompleteFriend::ResetStartColor()
{
	m_vwp_head.lock()->GetGameComponent<BrightnessController>()->ResetMeshDraw();
	m_vwp_body.lock()->GetGameComponent<BrightnessController>()->ResetMeshDraw();
	m_vwp_heart.lock()->GetGameComponent<BrightnessController>()->ResetMeshDraw();
	m_vwp_eye.lock()->GetGameComponent<BrightnessController>()->ResetMeshDraw();
	m_vwp_nose.lock()->GetGameComponent<BrightnessController>()->ResetMeshDraw();
	m_vwp_mouth.lock()->GetGameComponent<BrightnessController>()->ResetMeshDraw();

	m_vwp_head.lock()->GetGameComponent<BrightnessController>()->ResetStartColor();
	m_vwp_body.lock()->GetGameComponent<BrightnessController>()->ResetStartColor();
	m_vwp_heart.lock()->GetGameComponent<BrightnessController>()->ResetStartColor();
	m_vwp_eye.lock()->GetGameComponent<BrightnessController>()->ResetStartColor();
	m_vwp_nose.lock()->GetGameComponent<BrightnessController>()->ResetStartColor();
	m_vwp_mouth.lock()->GetGameComponent<BrightnessController>()->ResetStartColor();
	if (m_vwp_helmet.lock())
	{
		m_vwp_helmet.lock()->GetGameComponent<BrightnessController>()->ResetMeshDraw();
		m_vwp_helmet.lock()->GetGameComponent<BrightnessController>()->ResetStartColor();
	}
	if (m_vwp_chara.lock())
	{
		m_vwp_chara.lock()->GetGameComponent<BrightnessController>()->ResetMeshDraw();
		m_vwp_chara.lock()->GetGameComponent<BrightnessController>()->ResetStartColor();
	}
}

void ButiEngine::CompleteFriend::SetBrightness(const float arg_brightness)
{
	m_vwp_head.lock()->GetGameComponent<BrightnessController>()->SetBrightness(arg_brightness);
	m_vwp_body.lock()->GetGameComponent<BrightnessController>()->SetBrightness(arg_brightness);
	m_vwp_heart.lock()->GetGameComponent<BrightnessController>()->SetBrightness(arg_brightness);
	m_vwp_eye.lock()->GetGameComponent<BrightnessController>()->SetBrightness(arg_brightness);
	m_vwp_nose.lock()->GetGameComponent<BrightnessController>()->SetBrightness(arg_brightness);
	m_vwp_mouth.lock()->GetGameComponent<BrightnessController>()->SetBrightness(arg_brightness);
	if (m_vwp_helmet.lock())
	{
		m_vwp_helmet.lock()->GetGameComponent<BrightnessController>()->SetBrightness(arg_brightness);
	}
	if (m_vwp_chara.lock())
	{
		m_vwp_chara.lock()->GetGameComponent<BrightnessController>()->SetBrightness(arg_brightness);
	}
}

void ButiEngine::CompleteFriend::StopDance()
{
	m_isDance = false;
}

void ButiEngine::CompleteFriend::StartBlowAnimation()
{
	m_isBlow = true;
	m_vlp_animationController->ChangeAnimation(5.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[8]->GetAnimation());
}

void ButiEngine::CompleteFriend::CreateHead(Value_weak_ptr<Transform> arg_vwp_transform)
{
	m_vwp_head = GetManager().lock()->AddObjectFromCereal("CompleteFriend_Head", arg_vwp_transform);
	auto bone = m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>()->GetBone()->searchBoneByName("head");
	m_vwp_head.lock()->transform->SetBaseTransform(m_vwp_body.lock()->transform, true);
	m_vwp_head.lock()->transform->SetBaseTransform(bone->transform);
}

void ButiEngine::CompleteFriend::CreateBody(Value_weak_ptr<Transform> arg_vwp_transform)
{
	m_vwp_body = GetManager().lock()->AddObjectFromCereal("CompleteFriend_Body", arg_vwp_transform);
	m_vwp_body.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	m_vwp_body.lock()->transform->SetLocalPosition(Vector3Const::Zero);

	auto modelDraw = m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>();
	m_vlp_animationController = ButiRendering::CreateAnimationController(modelDraw->GetBone());

	auto heartBone = modelDraw->GetBone()->searchBoneByName("Heart");
	m_vwp_heart = GetManager().lock()->AddObjectFromCereal("FriendHeart");
	m_vwp_heart.lock()->SetObjectName("CompleteFriend_Heart");
	m_vwp_heart.lock()->transform->SetLocalPosition(Vector3Const::Zero);
	m_vwp_heart.lock()->transform->SetBaseTransform(heartBone->transform, true);
}

void ButiEngine::CompleteFriend::CreateEye(Value_weak_ptr<Transform> arg_vwp_transform, const Rank arg_rank)
{
	m_vwp_eye = GetManager().lock()->AddObjectFromCereal("CompleteFriend_Eyes", arg_vwp_transform);
	m_vwp_eye.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform, true);

	if (arg_rank == Rank::Normal)
	{
		m_vwp_eye.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform()->SetLocalScale(1.0f);
	}
	else if (arg_rank == Rank::Bad)
	{
		m_vwp_eye.lock()->GetGameComponent<MeshDrawComponent>(2)->GetTransform()->SetLocalScale(1.0f);
	}
}

void ButiEngine::CompleteFriend::CreateNose(Value_weak_ptr<Transform> arg_vwp_transform, const Rank arg_rank)
{
	m_vwp_nose = GetManager().lock()->AddObjectFromCereal("CompleteFriend_Nose", arg_vwp_transform);
	m_vwp_nose.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform, true);
	if (arg_rank == Rank::Good)
	{
		m_vwp_nose.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform()->SetLocalScale(1.0f);
	}
	else if (arg_rank == Rank::Normal)
	{
		m_vwp_nose.lock()->GetGameComponent<MeshDrawComponent>(0)->GetTransform()->SetLocalScale(1.0f);
	}
	else if (arg_rank == Rank::Bad)
	{
		m_vwp_nose.lock()->GetGameComponent<MeshDrawComponent>(2)->GetTransform()->SetLocalScale(1.0f);
	}
}

void ButiEngine::CompleteFriend::CreateMouth(Value_weak_ptr<Transform> arg_vwp_transform, const Rank arg_rank)
{
	m_vwp_mouth = GetManager().lock()->AddObjectFromCereal("CompleteFriend_Mouth", arg_vwp_transform);
	m_vwp_mouth.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform, true);
	if (arg_rank == Rank::Good)
	{
		m_vwp_mouth.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform()->SetLocalScale(1.0f);
	}
	else if (arg_rank == Rank::Normal)
	{
		m_vwp_mouth.lock()->GetGameComponent<MeshDrawComponent>(0)->GetTransform()->SetLocalScale(1.0f);
	}
	else if (arg_rank == Rank::Bad)
	{
		m_vwp_mouth.lock()->GetGameComponent<MeshDrawComponent>(2)->GetTransform()->SetLocalScale(1.0f);
	}
}

void ButiEngine::CompleteFriend::CreateHelmet()
{
	m_vwp_helmet = GetManager().lock()->AddObjectFromCereal("CompleteFriend_Helmet", ObjectFactory::Create<Transform>());
	m_vwp_helmet.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform, true);
}

void ButiEngine::CompleteFriend::CreateChara()
{
	m_vwp_chara = GetManager().lock()->AddObjectFromCereal("Chara_Kami");
	m_vwp_chara.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform, true);

	if (m_vwp_friendData.lock()->hasHelmet)
	{
		auto anim = m_vwp_chara.lock()->GetGameComponent<PositionAnimation>();
		auto targetPos = anim->GetTargetPosition();
		targetPos.y += 0.3f;
		anim->SetTargetPosition(targetPos);
	}
}
