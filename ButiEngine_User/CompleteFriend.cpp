#include "stdafx_u.h"
#include "CompleteFriend.h"
#include "FriendManager.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"

void ButiEngine::CompleteFriend::OnUpdate()
{
	m_vlp_animationController->Update();
}

void ButiEngine::CompleteFriend::OnSet()
{
}

void ButiEngine::CompleteFriend::OnRemove()
{
}

void ButiEngine::CompleteFriend::OnShowUI()
{
}

void ButiEngine::CompleteFriend::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::CompleteFriend::Clone()
{
	return ObjectFactory::Create<CompleteFriend>();
}

void ButiEngine::CompleteFriend::CreateParts(Value_weak_ptr<FriendData> arg_vwp_friendData)
{
	m_vwp_body = GetManager().lock()->AddObjectFromCereal("CompleteFriend_Body", arg_vwp_friendData.lock()->vlp_bodyTransform);
	m_vwp_body.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	m_vwp_body.lock()->transform->SetLocalPosition(Vector3Const::Zero);

	auto modelDraw = m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>();
	m_vlp_animationController = ButiRendering::CreateAnimationController(modelDraw->GetBone());

	auto heartBone = modelDraw->GetBone()->searchBoneByName("Heart");
	auto heart = GetManager().lock()->AddObjectFromCereal("FriendHeart");
	heart.lock()->transform->SetLocalPosition(Vector3Const::Zero);
	heart.lock()->transform->SetBaseTransform(heartBone->transform, true);

	m_vwp_head = GetManager().lock()->AddObjectFromCereal("CompleteFriend_Head", arg_vwp_friendData.lock()->vlp_headTransform);
	auto bone = m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>()->GetBone()->searchBoneByName("head");
	m_vwp_head.lock()->transform->SetBaseTransform(m_vwp_body.lock()->transform, true);
	m_vwp_head.lock()->transform->SetBaseTransform(bone->transform);

	//auto eye = GetManager().lock()->AddObjectFromCereal("CompleteFriend_Eyes", arg_vwp_friendData.lock()->vlp_eyeTransform);
	//eye.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform, true);
	//if (!arg_vwp_friendData.lock()->isExact_Eye)
	//{
	//	eye.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform()->SetLocalScale(1.0f);
	//}

	//auto nose = GetManager().lock()->AddObjectFromCereal("CompleteFriend_Nose", arg_vwp_friendData.lock()->vlp_noseTransform);
	//nose.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform, true);
	//if (arg_vwp_friendData.lock()->isExact_Nose)
	//{
	//	nose.lock()->GetGameComponent<MeshDrawComponent>(0)->GetTransform()->SetLocalScale(1.0f);
	//}
	//else
	//{
	//	nose.lock()->GetGameComponent<MeshDrawComponent>(0)->GetTransform()->SetLocalScale(1.0f);
	//}

	//auto mouth = GetManager().lock()->AddObjectFromCereal("CompleteFriend_Mouth", arg_vwp_friendData.lock()->vlp_mouthTransform);
	//mouth.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform, true);
	//if (arg_vwp_friendData.lock()->isExact_Mouth)
	//{
	//	mouth.lock()->GetGameComponent<MeshDrawComponent>(0)->GetTransform()->SetLocalScale(1.0f);
	//}
	//else
	//{
	//	mouth.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform()->SetLocalScale(1.0f);
	//}
}

void ButiEngine::CompleteFriend::StartDance()
{
	m_vlp_animationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[1]->GetAnimation());

	m_vlp_animationController->GetCurrentModelAnimation()->SetIsLoop(true);
}
