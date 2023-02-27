#include "stdafx_u.h"
#include "FriendHead.h"
#include "InputManager.h"
#include "GameSettings.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "PauseManager.h"
#include "FriendManager.h"
#include "FriendBody.h"
#include "ScoreManager.h"
#include "FriendHead_PartHitArea.h"
#include "GameLevelManager.h"
#include "FriendHead_Center.h"
#include "StageManager.h"
#include "TutorialManager.h"
#include "GameCamera.h"
#include "FriendFacePart.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/TriggerComponent.h"

void ButiEngine::FriendHead::OnUpdate()
{
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::G))
	{
		m_isShowGuide = !m_isShowGuide;
		if (m_isShowGuide)
		{
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform()->SetLocalScale(1.0f);
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(2)->GetTransform()->SetLocalScale(1.0f);
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(3)->GetTransform()->SetLocalScale(1.0f);
		}
		else
		{
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform()->SetLocalScale(0.0f);
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(2)->GetTransform()->SetLocalScale(0.0f);
			gameObject.lock()->GetGameComponent<MeshDrawComponent>(3)->GetTransform()->SetLocalScale(0.0f);
		}
	}


	if (!CanUpdate())
	{
		return;
	}

	if (m_isDisappear)
	{
		OnDisappear();
		return;
	}

	Appear();

	if (m_vlp_appearTimer->IsOn())
	{
		return;
	}

	if (m_vlp_completeFaceCountUpTimer->Update())
	{
		m_vlp_completeFaceCountUpTimer->Stop();
		//m_isFast = false;
	}

	//if (m_isFast)
	//{
	//	if (m_isTutorial)
	//	{
	//		if (m_vlp_spawnStarFlashIntervalTimer->Update())
	//		{
	//			SpawnStarFlash();
	//		}
	//	}
	//	else if (m_vwp_stageManager.lock()->IsGameStart())
	//	{
	//		if (m_vlp_spawnStarFlashIntervalTimer->Update())
	//		{
	//			SpawnStarFlash();
	//		}
	//	}
	//}

	if (!m_isCompleteFace && CanPut())
	{
		CompleteFace();
	}

	Control();
	CalcVelocity();
	CheckPut();
}

void ButiEngine::FriendHead::OnSet()
{
}

void ButiEngine::FriendHead::OnRemove()
{
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("FriendHead"));

	if (m_vwp_headCenter.lock())
	{
		m_vwp_headCenter.lock()->SetIsRemove(true);
	}
	if (m_vwp_eyesHitArea.lock())
	{
		m_vwp_eyesHitAreaComponent.lock()->Dead();
	}
	if (m_vwp_noseHitArea.lock())
	{
		m_vwp_noseHitAreaComponent.lock()->Dead();
	}
	if (m_vwp_mouthHitArea.lock())
	{
		m_vwp_mouthHitAreaComponent.lock()->Dead();
	}
	if (m_vwp_dummyHitArea.lock())
	{
		m_vwp_dummyHitAreaComponent.lock()->Dead();
	}
}

void ButiEngine::FriendHead::OnShowUI()
{
	GUI::DragInt("TrackerIndex", m_trackerIndex, 1.0, 0, 16);

	GUI::BulletText(U8("完成時「はやい」ボーナスが付く時間"));
	GUI::DragInt("##fastBorder", m_fastBorder, 1.0f, 0, 1000);
}

void ButiEngine::FriendHead::Start()
{
	auto tutorialManager = GetManager().lock()->GetGameObject("TutorialManager");
	if (tutorialManager.lock())
	{
		m_vwp_tutorialManager = tutorialManager.lock()->GetGameComponent<TutorialManager>();
		m_isTutorial = true;
	}

	if (!m_isTutorial)
	{
		m_vwp_stageManager = GetManager().lock()->GetGameObject("StageManager").lock()->GetGameComponent<StageManager>();
	}
	m_vwp_inputManager = GetManager().lock()->GetGameObject("InputManager").lock()->GetGameComponent<InputManager>();
	m_vwp_gameSettings = GetManager().lock()->GetGameObject("GameSettings").lock()->GetGameComponent<GameSettings>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_friendManager = GetManager().lock()->GetGameObject("FriendManager").lock()->GetGameComponent<FriendManager>();

	m_vwp_headCenter = GetManager().lock()->AddObjectFromCereal("HeadCenter");
	m_vwp_headCenter.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	m_vwp_headCenterComponent = m_vwp_headCenter.lock()->GetGameComponent<FriendHead_Center>();

	m_prevPos = Vector3Const::Zero;
	m_crntPos = Vector3Const::Zero;
	m_velocity = Vector3Const::Zero;

	m_vlp_appearTimer = ObjectFactory::Create<RelativeTimer>(10);
	m_vlp_appearTimer->Start();

	m_isPut = false;

	//m_isFast = true;
	//if (m_isTutorial && m_vwp_tutorialManager.lock()->GetTutorialPhase() != 2)
	//{
	//	m_isFast = false;
	//}

	m_vlp_completeFaceCountUpTimer = ObjectFactory::Create<RelativeTimer>(m_fastBorder);

	m_vlp_spawnStarFlashIntervalTimer = ObjectFactory::Create<RelativeTimer>(3);

	gameObject.lock()->GetGameComponent<MeshDrawComponent>(1)->GetTransform()->SetLocalScale(0.0f);
	gameObject.lock()->GetGameComponent<MeshDrawComponent>(2)->GetTransform()->SetLocalScale(0.0f);
	gameObject.lock()->GetGameComponent<MeshDrawComponent>(3)->GetTransform()->SetLocalScale(0.0f);

	m_isDisappear = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendHead::Clone()
{
	auto clone = ObjectFactory::Create<FriendHead>();
	clone->m_trackerIndex = m_trackerIndex;
	clone->m_fastBorder = m_fastBorder;
	return clone;
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::FriendHead::GetEye()
{
	return m_vwp_eyesHitAreaComponent.lock()->GetPart();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::FriendHead::GetNose()
{
	return m_vwp_noseHitAreaComponent.lock()->GetPart();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::FriendHead::GetMouth()
{
	return m_vwp_mouthHitAreaComponent.lock()->GetPart();
}

std::int32_t ButiEngine::FriendHead::GetEyeScore()
{
	auto gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	std::int32_t gameLevel = gameLevelManager->GetGameLevel();

	if (gameLevel == 0)
	{
		return 0;
	}

	std::int32_t score = 0;
	//score += m_vwp_eyesHitAreaComponent.lock()->GetCalcScore();
	return score;
}

std::int32_t ButiEngine::FriendHead::GetNoseScore()
{
	auto gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	std::int32_t gameLevel = gameLevelManager->GetGameLevel();

	if (gameLevel == 0)
	{
		return 0;
	}

	return 0;// m_vwp_noseHitAreaComponent.lock()->GetCalcScore();
}

std::int32_t ButiEngine::FriendHead::GetMouthScore()
{
	auto gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	std::int32_t gameLevel = gameLevelManager->GetGameLevel();

	if (gameLevel == 0)
	{
		return 0;
	}

	return 0;// m_vwp_mouthHitAreaComponent.lock()->GetCalcScore();
}

bool ButiEngine::FriendHead::IsFast()
{
	//if (!m_isFast)
	//{
	//	return false;
	//}

	//auto gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	//std::int32_t gameLevel = gameLevelManager->GetGameLevel();

	//if (!m_isTutorial && gameLevel == 0)
	//{
	//	return false;
	//}

	//return m_isFast;
	return false;
}

bool ButiEngine::FriendHead::IsGood()
{
	auto gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	std::int32_t gameLevel = gameLevelManager->GetGameLevel();

	if (!m_isTutorial && gameLevel == 0)
	{
		return false;
	}
	
	if (GetEye().lock()->GetGameComponent<FriendFacePart>()->GetPartRank() != Rank::Good)
	{
		return false;
	}
	if (GetNose().lock()->GetGameComponent<FriendFacePart>()->GetPartRank() != Rank::Good)
	{
		return false;
	}
	if (GetMouth().lock()->GetGameComponent<FriendFacePart>()->GetPartRank() != Rank::Good)
	{
		return false;
	}
	return true;
}

bool ButiEngine::FriendHead::IsExistPartStuckArea()
{
	return GetPartStuckAreas().size() > 0;
}

void ButiEngine::FriendHead::LeavePartRandom()
{
	if (m_isPut)
	{
		return;
	}

	GetManager().lock()->GetGameObject("CameraParent").lock()->GetGameComponent<GameCamera>()->StartShake(15);

	auto partStuckAreas = GetPartStuckAreas();
	std::int32_t maxRandom = partStuckAreas.size() - 1;
	std::int32_t leavePartAreaIndex = ButiRandom::GetInt(0, maxRandom);

	partStuckAreas[leavePartAreaIndex].lock()->LeavePart();

	if (m_isCompleteFace)
	{
		auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
		meshDraw->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		meshDraw->SetMaterialTag(MaterialTag("Material/FriendHead_Gray.mat"), 0);
		meshDraw->ReRegist();

		m_vlp_completeFaceCountUpTimer->Reset();
		m_vlp_completeFaceCountUpTimer->Stop();
		m_vlp_spawnStarFlashIntervalTimer->Reset();
		m_vlp_spawnStarFlashIntervalTimer->Stop();

		m_isCompleteFace = false;

		//m_isFast = true;
	}
}

void ButiEngine::FriendHead::Dead()
{
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("FriendHead"));

	if (m_vwp_headCenter.lock())
	{
		m_vwp_headCenter.lock()->SetIsRemove(true);
	}
	if (m_vwp_eyesHitArea.lock())
	{
		m_vwp_eyesHitAreaComponent.lock()->Dead(true);
	}
	if (m_vwp_noseHitArea.lock())
	{
		m_vwp_noseHitAreaComponent.lock()->Dead(true);
	}
	if (m_vwp_mouthHitArea.lock())
	{
		m_vwp_mouthHitAreaComponent.lock()->Dead(true);
	}
	if (m_vwp_dummyHitArea.lock())
	{
		m_vwp_dummyHitAreaComponent.lock()->Dead(true);
	}

	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::FriendHead::Disappear()
{
	m_isDisappear = true;

	if (m_vwp_eyesHitArea.lock())
	{
		m_vwp_eyesHitAreaComponent.lock()->RemoveGuideMarker();
	}
	if (m_vwp_noseHitArea.lock())
	{
		m_vwp_noseHitAreaComponent.lock()->RemoveGuideMarker();
	}
	if (m_vwp_mouthHitArea.lock())
	{
		m_vwp_mouthHitAreaComponent.lock()->RemoveGuideMarker();
	}
	
	RemoveTriggerComponent();
	AddScaleAnimation(0.0f, Easing::EasingType::EaseInBack);
}

void ButiEngine::FriendHead::Control()
{
	if (GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize() > m_trackerIndex)
	{
		ControlByVRTracker();
	}

	ControlByKeyboard();
	ControlByGamePad();
}

void ButiEngine::FriendHead::ControlByKeyboard()
{
	Vector3 direction = Vector3Const::Zero;

	if (GameDevice::GetInput().CheckKey(ButiInput::Keys::D))
	{
		direction.x = 1.0f;
	}
	else if (GameDevice::GetInput().CheckKey(ButiInput::Keys::A))
	{
		direction.x = -1.0f;
	}
	if (GameDevice::GetInput().CheckKey(ButiInput::Keys::W))
	{
		direction.y = 1.0f;
	}
	else if (GameDevice::GetInput().CheckKey(ButiInput::Keys::S))
	{
		direction.y = -1.0f;
	}

	direction.x *= -1.0f;
	direction.Normalize();

	constexpr float moveSpeed = 0.1f;

	Vector3 velocity = direction * moveSpeed * GameDevice::GetWorldSpeed();
	gameObject.lock()->transform->Translate(velocity);
}

void ButiEngine::FriendHead::ControlByGamePad()
{
	Vector3 direction = Vector3Const::Zero;

	Vector2 leftStick = m_vwp_inputManager.lock()->GetLeftStick();
	if (leftStick.GetLengthSqr() != 0.0f)
	{
		direction = leftStick;
	}

	direction.x *= -1.0f;
	direction.Normalize();

	constexpr float moveSpeed = 0.1f;

	Vector3 velocity = direction * moveSpeed * GameDevice::GetWorldSpeed();
	gameObject.lock()->transform->Translate(velocity);

	if (GameDevice::GetInput().GetPadButton(ButiInput::PadButtons::XBOX_BUTTON_RIGHT))
	{
		gameObject.lock()->transform->RollLocalRotationY_Degrees(-2.0f);
	}
	else if (GameDevice::GetInput().GetPadButton(ButiInput::PadButtons::XBOX_BUTTON_LEFT))
	{
		gameObject.lock()->transform->RollLocalRotationY_Degrees(2.0f);
	}
}

void ButiEngine::FriendHead::ControlByVRTracker()
{
	gameObject.lock()->transform->SetLocalPosition(m_vwp_gameSettings.lock()->GetTrackerPos(m_trackerIndex));
	gameObject.lock()->transform->SetLocalRotation(m_vwp_gameSettings.lock()->GetTrackerRotation(m_trackerIndex));
}

void ButiEngine::FriendHead::SpawnStarFlash()
{
	Vector3 center = m_vwp_headCenter.lock()->transform->GetWorldPosition();
	center += 1.0f * Vector3Const::ZAxis;

	Vector3 dir = Vector3Const::Zero;
	dir.x = ButiRandom::GetRandom(5.0f, 15.0f, 10);
	dir.y = ButiRandom::GetRandom(5.0f, 15.0f, 10);

	if (ButiRandom::GetInt(0, 1))
	{
		dir.x *= -1.0f;
	}
	if (ButiRandom::GetInt(0, 1))
	{
		dir.y *= -1.0f;
	}

	dir.Normalize();

	float radius = 0.75f;
	Vector3 pos = center + dir * radius;
	auto starFlash = GetManager().lock()->AddObjectFromCereal("Effect_StarFlash");
	starFlash.lock()->transform->SetLocalPosition(pos);
}

void ButiEngine::FriendHead::OnPut(Value_weak_ptr<GameObject> arg_vwp_body)
{
	m_vwp_eyesHitAreaComponent.lock()->GetPart().lock()->GetGameComponent<FriendFacePart>()->RemoveStickAnimation();
	m_vwp_noseHitAreaComponent.lock()->GetPart().lock()->GetGameComponent<FriendFacePart>()->RemoveStickAnimation();
	m_vwp_mouthHitAreaComponent.lock()->GetPart().lock()->GetGameComponent<FriendFacePart>()->RemoveStickAnimation();

	arg_vwp_body.lock()->GetGameComponent<FriendBody>()->SetHead(gameObject);

	if (!m_isTutorial)
	{
		m_vwp_friendManager.lock()->AddFriendCount();
	}

	m_isPut = true;
}

void ButiEngine::FriendHead::CompleteFace()
{
	auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	meshDraw->SetColor(Vector4(1.0f, 0.83f, 0.71f, 1.0f));
	meshDraw->SetMaterialTag(MaterialTag("Material/FriendHead.mat"), 0);
	meshDraw->ReRegist();

	m_vlp_completeFaceCountUpTimer->Start();
	m_vlp_spawnStarFlashIntervalTimer->Start();

	m_isCompleteFace = true;
}

void ButiEngine::FriendHead::Appear()
{
	if (!m_vlp_appearTimer->IsOn())
	{
		return;
	}

	Vector3 targetPos = m_vwp_gameSettings.lock()->GetTrackerPos(m_trackerIndex);
	float progress = m_vlp_appearTimer->GetPercent();
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 newPos = MathHelper::LerpPosition(pos, targetPos, progress);
	gameObject.lock()->transform->SetLocalPosition(newPos);

	Quat targetRotation = m_vwp_gameSettings.lock()->GetTrackerRotation(m_trackerIndex).ToQuat();
	Quat rotation = gameObject.lock()->transform->GetLocalRotation().ToQuat();
	Quat newRotation = MathHelper::LearpQuat(rotation, targetRotation, progress);
	gameObject.lock()->transform->SetLocalRotation(newRotation);

	if (m_vlp_appearTimer->Update())
	{
		m_vlp_appearTimer->Stop();

		gameObject.lock()->transform->SetLocalPosition(m_vwp_gameSettings.lock()->GetTrackerPos(m_trackerIndex));
		gameObject.lock()->transform->SetLocalRotation(m_vwp_gameSettings.lock()->GetTrackerRotation(m_trackerIndex));

		CreatePartHitArea();
	}
}

void ButiEngine::FriendHead::OnDisappear()
{
	auto anim = gameObject.lock()->GetGameComponent<ScaleAnimation>();
	if (!anim)
	{
		m_isDisappear = false;
		Dead();
	}
}

void ButiEngine::FriendHead::AddScaleAnimation(const Vector3& arg_targetScale, Easing::EasingType arg_easeType)
{
	auto anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	anim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
	anim->SetTargetScale(arg_targetScale);
	anim->SetSpeed(1.0f / 30);
	anim->SetEaseType(arg_easeType);
}

void ButiEngine::FriendHead::CalcVelocity()
{
	m_prevPos = m_crntPos;
	m_crntPos = gameObject.lock()->transform->GetLocalPosition();

	m_velocity = m_crntPos - m_prevPos;
}

ButiEngine::Vector3 ButiEngine::FriendHead::GetTrackerPos()
{
	if (!(GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize() > m_trackerIndex))
	{
		return Vector3Const::Zero;
	}

	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_trackerIndex], deviceMatrix);
	Vector3 pos = deviceMatrix.GetPosition();
	pos *= m_vwp_gameSettings.lock()->GetCorrection();
	pos.x *= -1;
	pos.z = 0.0f;
	
	return pos;
}

ButiEngine::Matrix4x4 ButiEngine::FriendHead::GetTrackerRotation()
{
	if (!(GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize() > m_trackerIndex))
	{
		return Matrix4x4().Identity();
	}

	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_trackerIndex], deviceMatrix);
	auto rotation = deviceMatrix.GetRemovePosition();
	rotation._13 *= -1;
	rotation._31 *= -1;
	rotation._12 *= -1;
	rotation._21 *= -1;

	return rotation;
}

void ButiEngine::FriendHead::CheckPut()
{
	if (!CanPut())
	{
		return;
	}

	auto collisionBody = m_vwp_headCenterComponent.lock()->GetCollisionFriendBody();
	if (collisionBody.lock() && collisionBody.lock()->GetGameComponent<FriendBody>()->GetNeck().lock())
	{
		OnPut(collisionBody);
	}
}

bool ButiEngine::FriendHead::CanPut()
{
	if (!m_vwp_eyesHitAreaComponent.lock()->GetStickPart().lock())
	{
		return false;
	}
	if (!m_vwp_noseHitAreaComponent.lock()->GetStickPart().lock())
	{
		return false;
	}
	if (!m_vwp_mouthHitAreaComponent.lock()->GetStickPart().lock())
	{
		return false;
	}

	return true;
}

bool ButiEngine::FriendHead::CanUpdate()
{
	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return false;
	}

	if (m_isPut)
	{
		return false;
	}

	return true;
}

std::vector<ButiEngine::Value_weak_ptr<ButiEngine::FriendHead_PartHitArea>> ButiEngine::FriendHead::GetPartStuckAreas()
{
	std::vector<Value_weak_ptr<FriendHead_PartHitArea>> partStuckAreas;
	if (m_vwp_eyesHitAreaComponent.lock()->GetStickPart().lock())
	{
		partStuckAreas.push_back(m_vwp_eyesHitAreaComponent);
	}
	if (m_vwp_noseHitAreaComponent.lock()->GetStickPart().lock())
	{
		partStuckAreas.push_back(m_vwp_noseHitAreaComponent);
	}
	if (m_vwp_mouthHitAreaComponent.lock()->GetStickPart().lock())
	{
		partStuckAreas.push_back(m_vwp_mouthHitAreaComponent);
	}

	return partStuckAreas;
}

void ButiEngine::FriendHead::CreatePartHitArea()
{
	m_vwp_eyesHitArea = GetManager().lock()->AddObjectFromCereal("PartHitArea_Eyes");
	m_vwp_noseHitArea = GetManager().lock()->AddObjectFromCereal("PartHitArea_Nose");
	m_vwp_mouthHitArea = GetManager().lock()->AddObjectFromCereal("PartHitArea_Mouth");
	m_vwp_dummyHitArea = GetManager().lock()->AddObjectFromCereal("PartHitArea_Dummy");

	m_vwp_eyesHitAreaComponent = m_vwp_eyesHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();
	m_vwp_noseHitAreaComponent = m_vwp_noseHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();
	m_vwp_mouthHitAreaComponent = m_vwp_mouthHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();
	m_vwp_dummyHitAreaComponent = m_vwp_dummyHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();

	m_vwp_eyesHitAreaComponent.lock()->SetParent(gameObject);
	m_vwp_noseHitAreaComponent.lock()->SetParent(gameObject);
	m_vwp_mouthHitAreaComponent.lock()->SetParent(gameObject);
	m_vwp_dummyHitAreaComponent.lock()->SetParent(gameObject);

	auto eyesDefault = GetManager().lock()->GetGameObject("Eyes_Default");
	eyesDefault.lock()->transform->SetBaseTransform(m_vwp_headCenter.lock()->transform, true);

	auto noseDefault = GetManager().lock()->GetGameObject("Nose_Default");
	noseDefault.lock()->transform->SetBaseTransform(m_vwp_headCenter.lock()->transform, true);

	auto mouthDefault = GetManager().lock()->GetGameObject("Mouth_Default");
	mouthDefault.lock()->transform->SetBaseTransform(m_vwp_headCenter.lock()->transform, true);
}

void ButiEngine::FriendHead::RemoveTriggerComponent()
{
	auto triggerComponent = m_vwp_eyesHitArea.lock()->GetGameComponent<TriggerComponent>();
	triggerComponent->SetIsRemove(true);

	triggerComponent = m_vwp_noseHitArea.lock()->GetGameComponent<TriggerComponent>();
	triggerComponent->SetIsRemove(true);

	triggerComponent = m_vwp_mouthHitArea.lock()->GetGameComponent<TriggerComponent>();
	triggerComponent->SetIsRemove(true);

	triggerComponent = m_vwp_dummyHitArea.lock()->GetGameComponent<TriggerComponent>();
	triggerComponent->SetIsRemove(true);
}
