#include "stdafx_u.h"
#include "FriendBody.h"
#include "PauseManager.h"
#include "GameSettings.h"
#include "FriendCompleteDirecting.h"
#include "GameLevelManager.h"
#include "FriendBody_Neck.h"
#include "FriendManager.h"
#include "FriendHead.h"
#include "FriendBodySpawner.h"
#include "Header/GameObjects/DefaultGameComponent/RotationAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include "BonusFriend.h"
#include "TutorialManager.h"
#include "FadeOut.h"
#include "FriendFacePart.h"
#include "CompleteFriend.h"
#include "BombFriend.h"
#include "BlowFriend.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "FriendAligner.h"

void ButiEngine::FriendBody::OnUpdate()
{
	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return;
	}

	if (m_isBlow)
	{
		m_vlp_animationController->Update();
		return;
	}

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

	m_vwp_neck = GetManager().lock()->AddObjectFromCereal("FriendBody_Neck");
	m_vwp_neck.lock()->SetObjectName(gameObject.lock()->GetGameObjectName() + "_Neck");
	m_vwp_neck.lock()->GetGameComponent<FriendBody_Neck>()->SetParent(gameObject);

	auto modelDraw = gameObject.lock()->GetGameComponent<ModelDrawComponent>();
	auto bone = modelDraw->GetBone()->searchBoneByName("Heart");
	m_vwp_heart = GetManager().lock()->AddObjectFromCereal("FriendHeart");
	m_vwp_heart.lock()->transform->SetLocalPosition(Vector3Const::Zero);
	m_vwp_heart.lock()->transform->SetBaseTransform(bone->transform, true);

	m_vwp_guideHead = GetManager().lock()->AddObjectFromCereal("GuideHead");
	m_vwp_guideHead.lock()->SetObjectName(gameObject.lock()->GetGameObjectName() + "_GuideHead");
	m_vwp_guideHead.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);

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

	m_vlp_pullUpTimer = ObjectFactory::Create<RelativeTimer>(30);
	m_vlp_pullUpTimer->Start();

	m_offsetPos = gameObject.lock()->transform->GetLocalPosition();

	m_vlp_friendData->totalRank = Rank::NoRank;

	m_isBlow = false;

	isRemoveAfterMoveBack = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendBody::Clone()
{
	auto clone = ObjectFactory::Create<FriendBody>();
	clone->m_frontBorder = m_frontBorder;
	clone->m_vec_rotateSpeeds = m_vec_rotateSpeeds;
	clone->m_vec_moveHorizontalFrame = m_vec_moveHorizontalFrame;
	return clone;
}

ButiEngine::Rank ButiEngine::FriendBody::GetTotalRank()
{
	return m_vlp_friendData->totalRank;
}

void ButiEngine::FriendBody::SetHead(Value_weak_ptr<GameObject> arg_vwp_head)
{
	m_vwp_head = arg_vwp_head;

	auto modelDraw = gameObject.lock()->GetGameComponent<ModelDrawComponent>();
	auto bone = modelDraw->GetBone()->searchBoneByName("head");
	m_vwp_head.lock()->transform->SetBaseTransform(bone->transform);

	ChangeMaterial();
	CorrectionHead();
	SaveFriendData();
	CreateBonusFriend();

	gameObject.lock()->AddGameComponent<FriendCompleteDirecting>();
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("FriendBody"));

	RemoveNeck();
	RemoveGuideHead();
	StopBombTimer();

	if (m_vwp_friendBodySpawner.lock())
	{
		m_vwp_friendBodySpawner.lock()->DecreaceBodiesNumber();
	}
	StartMoveBackRandom();
	gameObject.lock()->GetGameComponent<CompleteFriend>()->StartDance();
}

bool ButiEngine::FriendBody::IsFront()
{
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	if (gameLevel == 0)
	{
		return false;
	}
	Vector3 front = GetFrontXZ(gameObject.lock()->transform->GetFront());
	Vector3 headFront = GetFrontXZ(m_vwp_head.lock()->transform->GetFront());
	float angle = abs(MathHelper::ToDegree(std::acos(front.Dot(headFront))));

	return angle <= m_frontBorder;
}

void ButiEngine::FriendBody::SetParameter(float arg_moveSpeed, float arg_rotateSpeed)
{
	m_moveSpeed = arg_moveSpeed;
	m_rotateSpeed = arg_rotateSpeed;
}

void ButiEngine::FriendBody::StartMoveBack(const Vector3& arg_targetPos, const std::int32_t arg_frame)
{
	m_isMoveHorizontal = false;

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	m_moveBackStartPos = pos;

	m_moveBackTargetPos = arg_targetPos;

	m_isMoveBack = true;
	m_vlp_moveBackTimer->ChangeCountFrame(arg_frame);
	m_vlp_moveBackTimer->Start();
}

void ButiEngine::FriendBody::RemoveNeck()
{
	if (m_vwp_neck.lock())
	{
		m_vwp_neck.lock()->SetIsRemove(true);
		m_vwp_neck = Value_weak_ptr<GameObject>();
	}
}

void ButiEngine::FriendBody::RemoveGuideHead()
{
	if (m_vwp_guideHead.lock())
	{
		m_vwp_guideHead.lock()->SetIsRemove(true);
		//m_vwp_guideHead.lock()->AddGameComponent<FadeOut>(120);
		//m_vwp_guideHead.lock()->AddGameComponent<SucideComponent>(120);
	}
}

void ButiEngine::FriendBody::RemoveHeart()
{
	if (m_vwp_heart.lock())
	{
		m_vwp_heart.lock()->SetIsRemove(true);
	}
}

void ButiEngine::FriendBody::Blow()
{
	if (m_isBlow)
	{
		return;
	}

	m_isBlow = true;

	auto modelDraw = gameObject.lock()->GetGameComponent<ModelDrawComponent>();
	m_vlp_animationController = ButiRendering::CreateAnimationController(modelDraw->GetBone());
	m_vlp_animationController->ChangeAnimation(5.0f, gameObject.lock()->GetResourceContainer()->
		GetModel(modelDraw->GetModelTag()).lock()->GetMotion()[8]->GetAnimation());
	gameObject.lock()->AddGameComponent<BlowFriend>();
}

void ButiEngine::FriendBody::Rotate()
{
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	if (gameLevel == 0)
	{
		return;
	}

	if (m_isMoveBack)
	{
		m_rotateSpeed = 10.0f;
	}
	gameObject.lock()->transform->RollLocalRotationY_Degrees(m_rotateSpeed * GameDevice::GetWorldSpeed());

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

	auto completeFriendComponent = gameObject.lock()->GetGameComponent<CompleteFriend>();
	if (completeFriendComponent)
	{
		completeFriendComponent->SetBrightness(MathHelper::Lerp(1.0f, 0.6f, progress));
	}

	if (m_vlp_moveBackTimer->Update())
	{
		m_vlp_moveBackTimer->Stop();
		m_isMoveBack = false;
		m_isStopRotate = true;

		gameObject.lock()->transform->SetLocalPosition(m_moveBackTargetPos);

		completeFriendComponent->SetBrightness(0.6f);

		if (isRemoveAfterMoveBack)
		{
			if (completeFriendComponent)
			{
				completeFriendComponent->Dead(true);
			}
		}
	}
}

void ButiEngine::FriendBody::StartMoveBackRandom()
{
	m_isMoveHorizontal = false;

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	m_moveBackStartPos = pos;

	std::int32_t targetPosIndex = FriendManager::GetCompleteFriendTargetPositionIndex();
	if (targetPosIndex > 0)
	{
		auto friendAligner = GetManager().lock()->GetGameObject("FriendAligner").lock()->GetGameComponent<FriendAligner>();
		if (friendAligner != nullptr)
		{
			gameObject.lock()->GetGameComponent<CompleteFriend>()->SetTargetPosIndex(targetPosIndex);
			m_moveBackTargetPos = friendAligner->GetCalcFriendPos(targetPosIndex);
		}
	}
	else
	{
		m_moveBackTargetPos = Vector3(0, 0, -60);
		m_moveBackTargetPos.x = ButiRandom::GetInt(-8, 8);
		isRemoveAfterMoveBack = true;
	}

	gameObject.lock()->GetGameComponent<CompleteFriend>()->ResetStartColor();

	m_isMoveBack = true;
	m_vlp_moveBackTimer->Start();
}

void ButiEngine::FriendBody::MoveHorizontal()
{
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	

	if (m_vlp_pullUpTimer->Update())
	{
		m_vlp_pullUpTimer->Stop();
		m_vlp_pullUpTimer->SetCount(30);
	}

	Vector3 newPos = gameObject.lock()->transform->GetLocalPosition();

	newPos.x += m_moveSpeed * GameDevice::GetWorldSpeed();
	float progress = m_vlp_pullUpTimer->GetPercent();
	newPos.y = MathHelper::Lerp(m_offsetPos.y, 0, Easing::EaseOutElastic(progress));

	gameObject.lock()->transform->SetLocalPosition(newPos);

	if (abs(gameObject.lock()->transform->GetLocalPosition().x) > OUT_AREA_DISTANCE)
	{
		RemoveNeck();
		RemoveGuideHead();
		RemoveHeart();
		gameObject.lock()->SetIsRemove(true);
		if (m_vwp_friendBodySpawner.lock())
		{
			m_vwp_friendBodySpawner.lock()->DecreaceBodiesNumber();
		}
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

void ButiEngine::FriendBody::CheckTotalRank()
{
	float score = 0;
	score += static_cast<std::int32_t>(m_vlp_friendData->eyeRank);
	score += static_cast<std::int32_t>(m_vlp_friendData->noseRank);
	score += static_cast<std::int32_t>(m_vlp_friendData->mouthRank);
	score /= 3.0f;

	float goodBorder = 0.5f;
	float normalBorder = -0.5f;

	if (score > goodBorder)
	{
		m_vlp_friendData->totalRank = Rank::Good;
	}
	else if (score >= normalBorder)
	{
		m_vlp_friendData->totalRank = Rank::Normal;
	}
	else
	{
		m_vlp_friendData->totalRank = Rank::Bad;
	}
}

void ButiEngine::FriendBody::CreateBonusFriend()
{
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	if (gameLevel == 0)
	{
		return;
	}

	std::int32_t createCount = 0;
	if (m_vlp_friendData->totalRank == Rank::Bad)
	{
		createCount = 0;
	}
	else if (m_vlp_friendData->totalRank == Rank::Normal)
	{
		createCount = 1;
	}
	else if (m_vlp_friendData->totalRank == Rank::Good)
	{
		createCount = 2;
	}

	for (std::int32_t i = 0; i < createCount; i++)
	{
		Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
		auto bonusFriend = GetManager().lock()->AddObjectFromCereal("BonusFriend", ObjectFactory::Create<Transform>(pos));
		bonusFriend.lock()->GetGameComponent<CompleteFriend>()->CreateParts(m_vlp_friendData);

		GetManager().lock()->GetGameObject("FriendManager").lock()->GetGameComponent<FriendManager>()->AddCompleteFriend(bonusFriend, m_vlp_friendData);

		auto bonusFriendComponent = bonusFriend.lock()->GetGameComponent<BonusFriend>();
		bonusFriendComponent->SetFrontBorder(m_frontBorder);
		m_vec_bonusFriends.push_back(bonusFriend);
	}
}

void ButiEngine::FriendBody::ChangeMaterial()
{
	auto modelDraw = gameObject.lock()->GetGameComponent<ModelDrawComponent>();
	modelDraw->SetColor(Vector4(1.0f, 0.83f, 0.71f, 1.0f));
	modelDraw->SetMaterialTag(MaterialTag("Material/FrendsBody.mat"), 0);
	modelDraw->ReRegist();
}

void ButiEngine::FriendBody::CorrectionHead()
{
	auto headCenter = GetManager().lock()->GetGameObject(GameObjectTag("HeadCenter"));
	Vector3 bodyPos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 headPos = m_vwp_head.lock()->transform->GetWorldPosition();
	headPos += bodyPos + headCenter.lock()->transform->GetLocalPosition() - headCenter.lock()->transform->GetWorldPosition();
	m_vwp_head.lock()->transform->SetWorldPosition(headPos);
}

void ButiEngine::FriendBody::StopBombTimer()
{
	auto bomb = gameObject.lock()->GetGameComponent<BombFriend>();
	if (bomb)
	{
		bomb->StopTimer();
	}
}

bool ButiEngine::FriendBody::IsFrontHead()
{
	if (!m_vwp_head.lock())
	{
		return IsFrontBody();
	}

	Vector3 front = GetFrontXZ(m_vwp_head.lock()->transform->GetFront());

	float angle = abs(MathHelper::ToDegree(std::acos(front.Dot(Vector3Const::ZAxis))));

	return angle <= m_frontBorder;
}

bool ButiEngine::FriendBody::IsFrontBody()
{
	Vector3 front = GetFrontXZ(gameObject.lock()->transform->GetFront());

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
	if (!m_vwp_head.lock())
	{
		return GetLookForwardBodyAngle();
	}

	Vector3 headFront = GetFrontXZ(m_vwp_head.lock()->transform->GetFront());
	float rollAngle = abs(MathHelper::ToDegree(std::acos(headFront.Dot(Vector3Const::ZAxis))));
	bool isClockwise = headFront.x >= 0.0f;
	if (isClockwise)
	{
		rollAngle *= -1.0f;
	}

	return rollAngle;
}

float ButiEngine::FriendBody::GetLookForwardBodyAngle()
{
	Vector3 front = GetFrontXZ(gameObject.lock()->transform->GetFront());
	float rollAngle = abs(MathHelper::ToDegree(std::acos(front.Dot(Vector3Const::ZAxis))));
	bool isClockwise = front.x >= 0.0f;
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

	m_vlp_friendData->vlp_headTransform = headTransform;
	m_vlp_friendData->vlp_bodyTransform = bodyTransform;

	auto headComponent = m_vwp_head.lock()->GetGameComponent<FriendHead>();
	auto eye = headComponent->GetEye();
	m_vlp_friendData->vlp_eyeTransform = eye.lock()->transform->Clone();

	auto nose = headComponent->GetNose();
	m_vlp_friendData->vlp_noseTransform = nose.lock()->transform->Clone();

	auto mouth = headComponent->GetMouth();
	m_vlp_friendData->vlp_mouthTransform = mouth.lock()->transform->Clone();

	auto helmet = headComponent->GetHelmet();
	m_vlp_friendData->hasHelmet = helmet.lock();

	m_vlp_friendData->eyeRank = headComponent->GetEye().lock()->GetGameComponent<FriendFacePart>()->GetPartRank();
	m_vlp_friendData->noseRank = headComponent->GetNose().lock()->GetGameComponent<FriendFacePart>()->GetPartRank();
	m_vlp_friendData->mouthRank = headComponent->GetMouth().lock()->GetGameComponent<FriendFacePart>()->GetPartRank();

	CheckTotalRank();

	Value_weak_ptr<GameObject> chara = Value_weak_ptr<GameObject>();
	if (m_vlp_friendData->totalRank == Rank::Good)
	{
		chara = GetManager().lock()->AddObjectFromCereal("Chara_Kami");
		chara.lock()->transform->SetBaseTransform(m_vwp_head.lock()->transform, true);
		if (helmet.lock())
		{
			auto anim = chara.lock()->GetGameComponent<PositionAnimation>();
			auto targetPos = anim->GetTargetPosition();
			targetPos.y += 0.3f;
			anim->SetTargetPosition(targetPos);
		}
	}

	GetManager().lock()->GetGameObject("FriendManager").lock()->GetGameComponent<FriendManager>()->AddCompleteFriend(gameObject, m_vlp_friendData);

	auto completeFriendComponent = gameObject.lock()->AddGameComponent<CompleteFriend>();
	completeFriendComponent->SetFriendData(m_vlp_friendData);
	completeFriendComponent->SetHead(m_vwp_head);
	completeFriendComponent->SetBody(gameObject);
	completeFriendComponent->SetHeart(m_vwp_heart);
	completeFriendComponent->SetEye(eye);
	completeFriendComponent->SetNose(nose);
	completeFriendComponent->SetMouth(mouth);
	if (helmet.lock())
	{
		completeFriendComponent->SetHelmet(helmet);
	}
	if (chara.lock())
	{
		completeFriendComponent->SetChara(chara);
	}

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
