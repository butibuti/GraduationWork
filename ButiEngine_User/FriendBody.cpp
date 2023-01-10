#include "stdafx_u.h"
#include "FriendBody.h"
#include "PauseManager.h"
#include "GameSettings.h"
#include "FriendCompleteDirecting.h"
#include "GameLevelManager.h"
#include "FriendBody_Neck.h"
#include "FriendManager.h"
#include "FriendHead.h"
#include "Header/GameObjects/DefaultGameComponent/RotationAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"

void ButiEngine::FriendBody::OnUpdate()
{
	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return;
	}

	m_vlp_animationController->Update();

	if (m_isRotate)
	{
		Rotate();
	}

	if (m_isMoveBack)
	{
		MoveBack();
	}

	if (m_isMoveHorizontal)
	{
		MoveHorizontal();
	}
}

void ButiEngine::FriendBody::OnSet()
{
}

void ButiEngine::FriendBody::OnRemove()
{
}

void ButiEngine::FriendBody::OnShowUI()
{
	GUI::BulletText("FrontBorder");
	GUI::DragFloat("##frontBorder", &m_frontBorder, 0.1f, 0.0f, 180.0f);

	if (!m_vwp_gameLevelManager.lock())
	{
		m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	}

	ResizeLevelParameter();

	std::int32_t maxLevel = m_vwp_gameLevelManager.lock()->GetMaxLevel();

	for (std::int32_t i = 1; i < maxLevel + 1; i++)
	{
		if (GUI::TreeNode("Level:" + std::to_string(i)))
		{
			GUI::BulletText("RotateSpeed");
			GUI::DragFloat("##rotateSpeed" + std::to_string(i), &m_vec_rotateSpeeds[i], 0.1f, 0.0f, 1000.0f);

			GUI::BulletText("MoveFrame");
			GUI::DragInt("##moveFrame" + std::to_string(i), &m_vec_moveHorizontalFrame[i], 1.0f, 1.0f, 1000.0f);

			GUI::TreePop();
		}
	}
}

void ButiEngine::FriendBody::Start()
{
	m_vwp_gameSettings = GetManager().lock()->GetGameObject("GameSettings").lock()->GetGameComponent<GameSettings>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();

	ResizeLevelParameter();

	m_vlp_animationController = ButiRendering::CreateAnimationController(gameObject.lock()->GetGameComponent<ModelDrawComponent>()->GetBone());

	m_vwp_neck = GetManager().lock()->AddObjectFromCereal("FriendBody_Neck");
	m_vwp_neck.lock()->SetObjectName(gameObject.lock()->GetGameObjectName() + "_Neck");
	m_vwp_neck.lock()->GetGameComponent<FriendBody_Neck>()->SetParent(gameObject);

	m_vlp_friendData = ObjectFactory::Create<FriendData>();

	m_isRotate = true;
	m_isStopRotate = false;

	m_isMoveBack = false;
	m_vlp_moveBackTimer = ObjectFactory::Create<RelativeTimer>(90);
	m_moveBackStartPos = Vector3Const::Zero;
	m_moveBackTargetPos = Vector3Const::Zero;

	m_isMoveHorizontal = true;
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	m_vlp_moveHorizontalTimer = ObjectFactory::Create<RelativeTimer>(m_vec_moveHorizontalFrame[gameLevel]);
	m_moveHorizontalStartPos = Vector3(7.0f, 0.5f, 0.0f);
	m_moveHorizontalTargetPos = Vector3(-7.0f, 0.5f, 0.0f);

	m_vlp_moveHorizontalTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendBody::Clone()
{
	auto clone = ObjectFactory::Create<FriendBody>();
	clone->m_frontBorder = m_frontBorder;
	clone->m_vec_rotateSpeeds = m_vec_rotateSpeeds;
	clone->m_vec_moveHorizontalFrame = m_vec_moveHorizontalFrame;
	return clone;
}

void ButiEngine::FriendBody::SetHead(Value_weak_ptr<GameObject> arg_vwp_head)
{
	m_vwp_head = arg_vwp_head;

	auto modelDraw = gameObject.lock()->GetGameComponent<ModelDrawComponent>();
	modelDraw->SetColor(Vector4(1.0f, 0.83f, 0.71f, 1.0f));
	modelDraw->SetMaterialTag(MaterialTag("Material/FriendsBody.mat"), 0);

	auto bone = modelDraw->GetBone()->searchBoneByName("head");

	m_vwp_head.lock()->transform->SetBaseTransform(bone->transform);

	auto headCenter = GetManager().lock()->GetGameObject(GameObjectTag("HeadCenter"));
	Vector3 bodyPos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 headPos = m_vwp_head.lock()->transform->GetWorldPosition();
	headPos += bodyPos + headCenter.lock()->transform->GetLocalPosition() - headCenter.lock()->transform->GetWorldPosition();
	m_vwp_head.lock()->transform->SetWorldPosition(headPos);

	SaveFriendData();

	m_isMoveHorizontal = false;
	gameObject.lock()->AddGameComponent<FriendCompleteDirecting>();
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("FriendBody"));
	
	if (m_vwp_neck.lock())
	{
		m_vwp_neck.lock()->SetIsRemove(true);
	}

	StartMoveBack();
	StartDance();
}

bool ButiEngine::FriendBody::IsFront()
{
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	if (gameLevel == 0)
	{
		return false;
	}
	Vector3 front = gameObject.lock()->transform->GetFront();
	float angle = abs(MathHelper::ToDegree(std::acos(front.Dot(Vector3Const::ZAxis))));

	return angle <= m_frontBorder;
}

void ButiEngine::FriendBody::Rotate()
{
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	if (gameLevel == 0)
	{
		return;
	}

	float rotateSpeed = m_vec_rotateSpeeds[gameLevel];
	if (m_isMoveBack)
	{
		rotateSpeed *= 2.0f;
	}
	gameObject.lock()->transform->RollLocalRotationY_Degrees(rotateSpeed * GameDevice::GetWorldSpeed());

	if (m_isStopRotate)
	{
		if (IsFrontHead())
		{
			StopRotate();
		}
	}
}

void ButiEngine::FriendBody::MoveBack()
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

		SpawnNewHead();
	}
}

void ButiEngine::FriendBody::StartMoveBack()
{
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	m_moveBackStartPos = pos;

	m_moveBackTargetPos.x = ButiRandom::GetInt(-5, 5);
	m_moveBackTargetPos.y = pos.y;
	m_moveBackTargetPos.z = pos.z + (ButiRandom::GetInt(-50, -45));

	m_isMoveBack = true;
	m_vlp_moveBackTimer->Start();
}

void ButiEngine::FriendBody::StartDance()
{
	m_vlp_animationController->ChangeAnimation(0.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(gameObject.lock()->GetGameComponent<ModelDrawComponent>()->GetModelTag()).lock()->GetMotion()[1]->GetAnimation());

	m_vlp_animationController->GetCurrentModelAnimation()->SetIsLoop(true);
}

void ButiEngine::FriendBody::MoveHorizontal()
{
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	if (gameLevel == 0)
	{
		return;
	}

	float progress = m_vlp_moveHorizontalTimer->GetPercent();
	Vector3 newPos = MathHelper::LerpPosition(m_moveHorizontalStartPos, m_moveHorizontalTargetPos, progress);

	gameObject.lock()->transform->SetLocalPosition(newPos);

	if (m_vlp_moveHorizontalTimer->Update())
	{
		m_vlp_moveHorizontalTimer->Stop();

		if (m_vwp_neck.lock())
		{
			m_vwp_neck.lock()->SetIsRemove(true);
		}
		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::FriendBody::SpawnNewHead()
{
	auto head = GetManager().lock()->AddObjectFromCereal("FriendHead");
	head.lock()->transform->SetLocalPosition(Vector3(0.0f, -10.0f, 0.0f));
}

void ButiEngine::FriendBody::StopRotate()
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

bool ButiEngine::FriendBody::IsFrontHead()
{
	if (!m_vwp_head.lock())
	{
		return false;
	}

	Vector3 front = GetFrontXZ(m_vwp_head.lock()->transform->GetFront());

	float angle = abs(MathHelper::ToDegree(std::acos(front.Dot(Vector3Const::ZAxis))));

	return angle <= m_frontBorder;
}

float ButiEngine::FriendBody::GetDifferenceFromHeadFront()
{
	Vector3 headFront = GetFrontXZ(m_vwp_head.lock()->transform->GetFront());
	Vector3 bodyFront = gameObject.lock()->transform->GetFront();

	float angle = abs(MathHelper::ToDegree(std::acos(bodyFront.Dot(headFront))));
	return angle;
}

float ButiEngine::FriendBody::GetLookForwardHeadAngle()
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

void ButiEngine::FriendBody::SaveFriendData()
{
	auto bodyTransform = gameObject.lock()->transform->Clone();
	auto headTransform = m_vwp_head.lock()->transform->Clone();
	headTransform->SetBaseTransform(bodyTransform);

	float rollAngle = GetLookForwardHeadAngle();
	bodyTransform->RollLocalRotationY_Degrees(rollAngle);

	m_vlp_friendData->vlp_headTransform = headTransform;
	m_vlp_friendData->vlp_bodyTransform = bodyTransform;

	auto headComponent = m_vwp_head.lock()->GetGameComponent<FriendHead>();
	m_vlp_friendData->vlp_eyeTransform = headComponent->GetEye().lock()->transform->Clone();
	m_vlp_friendData->vlp_noseTransform = headComponent->GetNose().lock()->transform->Clone();
	m_vlp_friendData->vlp_mouthTransform = headComponent->GetMouth().lock()->transform->Clone();

	auto vec_dummies = headComponent->GetDummies();
	auto end = vec_dummies.end();
	for (auto itr = vec_dummies.begin(); itr != end; ++itr)
	{
		m_vlp_friendData->vec_vlp_dummyTransforms.push_back((*itr).lock()->transform->Clone());
	}

	GetManager().lock()->GetGameObject("FriendManager").lock()->GetGameComponent<FriendManager>()->AddFriendData(m_vlp_friendData);
}

ButiEngine::Vector3 ButiEngine::FriendBody::GetFrontXZ(const Vector3& arg_front)
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

void ButiEngine::FriendBody::ResizeLevelParameter()
{
	std::int32_t maxLevel = m_vwp_gameLevelManager.lock()->GetMaxLevel();
	if (m_vec_rotateSpeeds.size() != (maxLevel + 1))
	{
		m_vec_rotateSpeeds.resize(maxLevel + 1);
	}

	if (m_vec_moveHorizontalFrame.size() != (maxLevel + 1))
	{
		m_vec_moveHorizontalFrame.resize(maxLevel + 1);
	}
}
