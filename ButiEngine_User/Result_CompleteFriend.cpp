#include "stdafx_u.h"
#include "Result_CompleteFriend.h"
#include "FriendManager.h"
#include "Result_FriendFallPoint.h"
#include "ResultManager.h"
#include "Result_BackHuman.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"

void ButiEngine::Result_CompleteFriend::OnUpdate()
{
	CheckFall();
	if (m_isFall)
	{
		Fall();
	}

	if (m_vlp_animationController)
	{
		m_vlp_animationController->Update();
	}
}

void ButiEngine::Result_CompleteFriend::OnSet()
{
}

void ButiEngine::Result_CompleteFriend::OnRemove()
{
}

void ButiEngine::Result_CompleteFriend::OnShowUI()
{
}

void ButiEngine::Result_CompleteFriend::Start()
{
	m_vwp_fallPoint = GetManager().lock()->GetGameObject("FallPoint");

	m_isFallStarted = false;
	m_isFall = false;
	m_velocity = Vector3Const::Zero;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Result_CompleteFriend::Clone()
{
	return ObjectFactory::Create<Result_CompleteFriend>();
}

void ButiEngine::Result_CompleteFriend::CreateParts(Value_weak_ptr<FriendData> arg_vwp_friendData)
{
	m_vwp_body = GetManager().lock()->AddObjectFromCereal("Result_FriendBody", arg_vwp_friendData.lock()->vlp_bodyTransform);
	m_vwp_body.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	m_vwp_body.lock()->transform->SetLocalPosition(Vector3Const::Zero);

	m_vlp_animationController = ButiRendering::CreateAnimationController(m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>()->GetBone());

	auto head = GetManager().lock()->AddObjectFromCereal("Result_FriendHead", arg_vwp_friendData.lock()->vlp_headTransform);
	auto bone = m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>()->GetBone()->searchBoneByName("head");
	head.lock()->transform->SetBaseTransform(m_vwp_body.lock()->transform, true);
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

void ButiEngine::Result_CompleteFriend::Fall()
{
	constexpr float gravity = 0.05f;

	m_velocity.y -= gravity;

	Vector3 pos = gameObject.lock()->transform->Translate(m_velocity);

	if (pos.y <= 0.0f)
	{
		gameObject.lock()->transform->SetLocalPositionY(0.0f);
		m_isFall = false;
	}
}

void ButiEngine::Result_CompleteFriend::CheckFall()
{
	if (m_isFallStarted)
	{
		return;
	}

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 fallPointPos = m_vwp_fallPoint.lock()->transform->GetLocalPosition();

	if (pos.x >= fallPointPos.x)
	{
		gameObject.lock()->RemoveGameObjectTag(GameObjectTag("NotFalling"));
		auto notFallingFriends = GetManager().lock()->GetGameObjects(GameObjectTag("NotFalling"));
		if (notFallingFriends.GetSize() == 0)
		{
			m_vwp_fallPoint.lock()->GetGameComponent<Result_FriendFallPoint>()->StopMove();
			GetManager().lock()->GetGameObject("ResultManager").lock()->GetGameComponent<ResultManager>()->StartFailedTimer();
		}

		GetManager().lock()->GetGameObject("BackHuman_Body").lock()->GetGameComponent<Result_BackHuman>()->AdvanceFacePartAnimation();

		m_vlp_animationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
			GetModel(m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[1]->GetAnimation());

		m_vlp_animationController->GetCurrentModelAnimation()->SetIsLoop(true);

		m_isFallStarted = true;
		m_isFall = true;
	}
}
