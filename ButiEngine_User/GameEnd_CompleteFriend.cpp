#include "stdafx_u.h"
#include "GameEnd_CompleteFriend.h"
#include "FriendManager.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"

void ButiEngine::GameEnd_CompleteFriend::OnUpdate()
{
	m_vlp_animationController->Update(0.5f);
}

void ButiEngine::GameEnd_CompleteFriend::OnSet()
{
}

void ButiEngine::GameEnd_CompleteFriend::OnRemove()
{
}

void ButiEngine::GameEnd_CompleteFriend::OnShowUI()
{
}

void ButiEngine::GameEnd_CompleteFriend::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameEnd_CompleteFriend::Clone()
{
	return ObjectFactory::Create<GameEnd_CompleteFriend>();
}

void ButiEngine::GameEnd_CompleteFriend::CreateParts(Value_weak_ptr<FriendData> arg_vwp_friendData)
{
	auto body = GetManager().lock()->AddObjectFromCereal("Result_FriendBody", arg_vwp_friendData.lock()->vlp_bodyTransform);
	body.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	body.lock()->transform->SetLocalPosition(Vector3Const::Zero);

	m_vlp_animationController = ButiRendering::CreateAnimationController(body.lock()->GetGameComponent<ModelDrawComponent>()->GetBone());
	m_vlp_animationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(body.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[1]->GetAnimation());
	m_vlp_animationController->GetCurrentModelAnimation()->SetIsLoop(true);

	auto modelDraw = body.lock()->GetGameComponent<ModelDrawComponent>();
	auto heartBone = modelDraw->GetBone()->searchBoneByName("Heart");
	auto heart = GetManager().lock()->AddObjectFromCereal("FriendHeart");
	heart.lock()->transform->SetLocalPosition(Vector3Const::Zero);
	heart.lock()->transform->SetBaseTransform(heartBone->transform, true);

	auto head = GetManager().lock()->AddObjectFromCereal("Result_FriendHead", arg_vwp_friendData.lock()->vlp_headTransform);
	auto bone = body.lock()->GetGameComponent<ModelDrawComponent>()->GetBone()->searchBoneByName("head");
	head.lock()->transform->SetBaseTransform(body.lock()->transform, true);
	head.lock()->transform->SetBaseTransform(bone->transform);

	auto eye = GetManager().lock()->AddObjectFromCereal("Result_FriendFacePart_Eyes", arg_vwp_friendData.lock()->vlp_eyeTransform);
	eye.lock()->transform->SetBaseTransform(head.lock()->transform, true);

	auto nose = GetManager().lock()->AddObjectFromCereal("Result_FriendFacePart_Nose", arg_vwp_friendData.lock()->vlp_noseTransform);
	nose.lock()->transform->SetBaseTransform(head.lock()->transform, true);

	auto mouth = GetManager().lock()->AddObjectFromCereal("Result_FriendFacePart_Mouth", arg_vwp_friendData.lock()->vlp_mouthTransform);
	mouth.lock()->transform->SetBaseTransform(head.lock()->transform, true);

	auto vec_dummyTransforms = arg_vwp_friendData.lock()->vec_vlp_dummyTransforms;
	auto end = vec_dummyTransforms.end();
	for (auto itr = vec_dummyTransforms.begin(); itr != end; ++itr)
	{
		auto dummy = GetManager().lock()->AddObjectFromCereal("Result_FriendFacePart_Dummy", (*itr));
		dummy.lock()->transform->SetBaseTransform(head.lock()->transform, true);
	}
}
