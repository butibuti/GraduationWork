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
#include "GameCamera.h"
#include "FriendFacePart.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/TriggerComponent.h"

void ButiEngine::FriendHead::OnUpdate()
{
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

	if (!m_isCompleteFace && CanPut())
	{
		CompleteFace();
	}

	Control();
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
	if (m_vwp_otherHitArea.lock())
	{
		m_vwp_otherHitAreaComponent.lock()->Dead();
	}
}

void ButiEngine::FriendHead::OnShowUI()
{
	GUI::DragInt("TrackerIndex", m_trackerIndex, 1.0, 0, 16);
}

void ButiEngine::FriendHead::Start()
{
	m_vwp_stageManager = GetManager().lock()->GetGameObject("StageManager").lock()->GetGameComponent<StageManager>();
	m_vwp_inputManager = GetManager().lock()->GetGameObject("InputManager").lock()->GetGameComponent<InputManager>();
	m_vwp_gameSettings = GetManager().lock()->GetGameObject("GameSettings").lock()->GetGameComponent<GameSettings>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_friendManager = GetManager().lock()->GetGameObject("FriendManager").lock()->GetGameComponent<FriendManager>();

	m_vwp_headCenter = GetManager().lock()->AddObjectFromCereal("HeadCenter");
	m_vwp_headCenter.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	m_vwp_headCenterComponent = m_vwp_headCenter.lock()->GetGameComponent<FriendHead_Center>();
	m_vwp_headCenterComponent.lock()->SetHead(gameObject);

	m_vlp_appearTimer = ObjectFactory::Create<RelativeTimer>(10);
	m_vlp_appearTimer->Start();

	m_isPut = false;

	m_isDisappear = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendHead::Clone()
{
	auto clone = ObjectFactory::Create<FriendHead>();
	clone->m_trackerIndex = m_trackerIndex;
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

bool ButiEngine::FriendHead::IsGood()
{
	auto gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	std::int32_t gameLevel = gameLevelManager->GetGameLevel();

	if (gameLevel == 0)
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

bool ButiEngine::FriendHead::ExistPartStuckArea()
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
	if (m_vwp_otherHitArea.lock())
	{
		m_vwp_otherHitAreaComponent.lock()->Dead(true);
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

void ButiEngine::FriendHead::OnPut(Value_weak_ptr<GameObject> arg_vwp_body)
{
	m_vwp_eyesHitAreaComponent.lock()->GetPart().lock()->GetGameComponent<FriendFacePart>()->RemoveStickAnimation();
	m_vwp_noseHitAreaComponent.lock()->GetPart().lock()->GetGameComponent<FriendFacePart>()->RemoveStickAnimation();
	m_vwp_mouthHitAreaComponent.lock()->GetPart().lock()->GetGameComponent<FriendFacePart>()->RemoveStickAnimation();

	arg_vwp_body.lock()->GetGameComponent<FriendBody>()->SetHead(gameObject);

	m_vwp_friendManager.lock()->AddFriendCount();

	m_isPut = true;
}

void ButiEngine::FriendHead::CompleteFace()
{
	auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	meshDraw->SetColor(Vector4(1.0f, 0.83f, 0.71f, 1.0f));
	meshDraw->SetMaterialTag(MaterialTag("Material/FriendHead.mat"), 0);
	meshDraw->ReRegist();

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
	m_vwp_otherHitArea = GetManager().lock()->AddObjectFromCereal("PartHitArea_Dummy");

	m_vwp_eyesHitAreaComponent = m_vwp_eyesHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();
	m_vwp_noseHitAreaComponent = m_vwp_noseHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();
	m_vwp_mouthHitAreaComponent = m_vwp_mouthHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();
	m_vwp_otherHitAreaComponent = m_vwp_otherHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();

	m_vwp_eyesHitAreaComponent.lock()->SetParent(gameObject);
	m_vwp_noseHitAreaComponent.lock()->SetParent(gameObject);
	m_vwp_mouthHitAreaComponent.lock()->SetParent(gameObject);
	m_vwp_otherHitAreaComponent.lock()->SetParent(gameObject);

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

	triggerComponent = m_vwp_otherHitArea.lock()->GetGameComponent<TriggerComponent>();
	triggerComponent->SetIsRemove(true);
}
