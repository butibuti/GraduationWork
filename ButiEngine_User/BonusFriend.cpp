#include "stdafx_u.h"
#include "BonusFriend.h"
#include "FriendManager.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/RotationAnimationComponent.h"

void ButiEngine::BonusFriend::OnUpdate()
{
	if (m_isDance)
	{
		m_vlp_animationController->Update();
	}

	if (m_isRotate)
	{
		Rotate();
	}

	if (m_isMoveBack)
	{
		MoveBack();
	}
}

void ButiEngine::BonusFriend::OnSet()
{
}

void ButiEngine::BonusFriend::OnRemove()
{
}

void ButiEngine::BonusFriend::OnShowUI()
{
}

void ButiEngine::BonusFriend::Start()
{
	m_vlp_moveBackTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_isMoveBack = false;
	m_isRotate = false;
	m_isStopRotate = false;
	m_isDance = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BonusFriend::Clone()
{
	return ObjectFactory::Create<BonusFriend>();
}

void ButiEngine::BonusFriend::CreateParts(Value_weak_ptr<FriendData> arg_vwp_friendData)
{
	m_vwp_body = GetManager().lock()->AddObjectFromCereal("Result_FriendBody", arg_vwp_friendData.lock()->vlp_bodyTransform);
	m_vwp_body.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	m_vwp_body.lock()->transform->SetLocalPosition(Vector3Const::Zero);

	m_vlp_animationController = ButiRendering::CreateAnimationController(m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>()->GetBone());
	m_vlp_animationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[1]->GetAnimation());
	m_vlp_animationController->GetCurrentModelAnimation()->SetProgress(0);

	m_vwp_head = GetManager().lock()->AddObjectFromCereal("Result_FriendHead", arg_vwp_friendData.lock()->vlp_headTransform);
	auto bone = m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>()->GetBone()->searchBoneByName("head");
	m_vwp_head.lock()->transform->SetBaseTransform(m_vwp_body.lock()->transform, true);
	m_vwp_head.lock()->transform->SetBaseTransform(bone->transform);

	auto eye = GetManager().lock()->AddObjectFromCereal("Result_FriendFacePart_Eyes", arg_vwp_friendData.lock()->vlp_eyeTransform);
	eye.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform, true);

	auto nose = GetManager().lock()->AddObjectFromCereal("Result_FriendFacePart_Nose", arg_vwp_friendData.lock()->vlp_noseTransform);
	nose.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform, true);

	auto mouth = GetManager().lock()->AddObjectFromCereal("Result_FriendFacePart_Mouth", arg_vwp_friendData.lock()->vlp_mouthTransform);
	mouth.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform, true);

	auto vec_dummyTransforms = arg_vwp_friendData.lock()->vec_vlp_dummyTransforms;
	auto end = vec_dummyTransforms.end();
	for (auto itr = vec_dummyTransforms.begin(); itr != end; ++itr)
	{
		auto dummy = GetManager().lock()->AddObjectFromCereal("Result_FriendFacePart_Dummy", (*itr));
		dummy.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform, true);
	}
}

void ButiEngine::BonusFriend::StartDance()
{
	m_isDance = true;
	m_vlp_animationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(m_vwp_body.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[1]->GetAnimation());
	m_vlp_animationController->GetCurrentModelAnimation()->SetIsLoop(true);
}

void ButiEngine::BonusFriend::MoveBack()
{
	float progress = m_vlp_moveBackTimer->GetPercent();
	Vector3 newPos = MathHelper::LerpPosition(m_moveBackStartPos, m_moveBackTargetPos, progress);

	gameObject.lock()->transform->SetLocalPosition(newPos);

	if (m_vlp_moveBackTimer->Update())
	{
		m_vlp_moveBackTimer->Stop();
		m_isMoveBack = false;
		m_isStopRotate = true;

		gameObject.lock()->transform->SetLocalPosition(m_moveBackTargetPos);
	}
}

void ButiEngine::BonusFriend::Rotate()
{
	float rotateSpeed = 10.0f;
	gameObject.lock()->transform->RollLocalRotationY_Degrees(rotateSpeed * GameDevice::GetWorldSpeed());

	if (m_isStopRotate)
	{
		if (IsFrontHead())
		{
			StopRotate();
		}
	}
}

void ButiEngine::BonusFriend::StopRotate()
{
	m_isRotate = false;

	auto target = gameObject.lock()->transform->Clone();
	float rollAngle = GetLookForwardHeadAngle();
	target->RollLocalRotationY_Degrees(rollAngle);

	auto anim = gameObject.lock()->AddGameComponent<RotationAnimation>();
	anim->SetInitRotate(gameObject.lock()->transform->GetLocalRotation());
	anim->SetTargetRotate(target->GetLocalRotation());
	anim->SetSpeed(1.0f / 10);
	anim->SetEaseType(Easing::EasingType::EaseOutQuad);
}

float ButiEngine::BonusFriend::GetLookForwardHeadAngle()
{
	Vector3 headFront = GetFrontXZ(m_vwp_head.lock()->transform->GetFront());
	float rollAngle = abs(MathHelper::ToDegree(std::acos(headFront.Dot(Vector3Const::ZAxis))));
	bool isClockwise = headFront.x >= 0.0f;
	if (isClockwise)
	{
		rollAngle *= -1.0f;
	}

	return rollAngle;
}

ButiEngine::Vector3 ButiEngine::BonusFriend::GetFrontXZ(const Vector3& arg_front)
{
	Vector3 front = arg_front;
	front.y = 0.0f;
	if (front == Vector3Const::Zero)
	{
		front.z = 1.0f;
	}
	front.Normalize();

	return front;
}

bool ButiEngine::BonusFriend::IsFrontHead()
{
	if (!m_vwp_head.lock())
	{
		return false;
	}

	Vector3 front = GetFrontXZ(m_vwp_head.lock()->transform->GetFront());

	float angle = abs(MathHelper::ToDegree(std::acos(front.Dot(Vector3Const::ZAxis))));

	return angle <= m_frontBorder;
}

void ButiEngine::BonusFriend::Appear(const std::int32_t arg_friendNum)
{
	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	anim->SetInitPosition(pos);

	Vector3 targetPos = pos;
	if (arg_friendNum == 0)
	{
		targetPos.x += -2.0f;
	}
	else if (arg_friendNum == 1)
	{
		targetPos.x -= 2.0f;
	}
	else if (arg_friendNum == 2)
	{
		targetPos.y += 2.0f;
		targetPos.z -= 2.0f;
	}

	anim->SetTargetPosition(targetPos);
	
	std::int32_t appearFrame = 15;
	anim->SetSpeed(1.0f / appearFrame);
	anim->SetEaseType(Easing::EasingType::EaseOutQuad);
}

void ButiEngine::BonusFriend::StartMoveBack()
{
	StartDance();

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	m_moveBackStartPos = pos;

	m_moveBackTargetPos.x = ButiRandom::GetInt(-5, 5);
	m_moveBackTargetPos.y = pos.y;
	m_moveBackTargetPos.z = pos.z + (ButiRandom::GetInt(-50, -45));

	m_isMoveBack = true;
	m_isRotate = true;
	m_vlp_moveBackTimer->Start();
}
