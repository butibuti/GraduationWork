#include "stdafx_u.h"
#include "Result_CompleteFriend.h"
#include "FriendManager.h"

void ButiEngine::Result_CompleteFriend::OnUpdate()
{
	CheckFall();
	if (m_isFall)
	{
		Fall();
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

	m_isFall = false;
	m_velocity = Vector3Const::Zero;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Result_CompleteFriend::Clone()
{
	return ObjectFactory::Create<Result_CompleteFriend>();
}

void ButiEngine::Result_CompleteFriend::CreateParts(Value_weak_ptr<FriendData> arg_vwp_friendData)
{
	auto body = GetManager().lock()->AddObjectFromCereal("Result_FriendBody", arg_vwp_friendData.lock()->vlp_bodyTransform);
	body.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	body.lock()->transform->SetLocalPosition(Vector3Const::Zero);

	auto head = GetManager().lock()->AddObjectFromCereal("Result_FriendHead", arg_vwp_friendData.lock()->vlp_headTransform);
	head.lock()->transform->SetBaseTransform(body.lock()->transform, true);

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
	constexpr float gravity = 0.01f;

	m_velocity.y -= gravity;

	Vector3 pos = gameObject.lock()->transform->Translate(m_velocity);

	if (pos.y <= 0.0f)
	{
		gameObject.lock()->transform->SetLocalPositionY(0.0f);
		m_isFall = false;
		isActive = false;
	}
}

void ButiEngine::Result_CompleteFriend::CheckFall()
{
	if (m_isFall)
	{
		return;
	}

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 fallPointPos = m_vwp_fallPoint.lock()->transform->GetLocalPosition();

	if (pos.x >= fallPointPos.x)
	{
		m_isFall = true;
	}
}
