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

void ButiEngine::FriendHead::OnUpdate()
{
	if (!CanUpdate())
	{
		return;
	}

	Control();
	CalcVelocity();
	CheckPut();

#ifdef DEBUG
	if (GameDevice::GetInput().CheckKey(ButiInput::Keys::B))
	{
		GetManager().lock()->AddObjectFromCereal("Box");
	}
#endif // DEBUG
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
	if (m_vwp_eyesHitAreaComponent.lock())
	{
		m_vwp_eyesHitAreaComponent.lock()->RemoveAllComponent();
	}
	if (m_vwp_noseHitAreaComponent.lock())
	{
		m_vwp_noseHitAreaComponent.lock()->RemoveAllComponent();
	}
	if (m_vwp_mouthHitAreaComponent.lock())
	{
		m_vwp_mouthHitAreaComponent.lock()->RemoveAllComponent();
	}
}

void ButiEngine::FriendHead::OnShowUI()
{
	GUI::DragInt("TrackerIndex", m_trackerIndex, 1.0, 0, 16);
}

void ButiEngine::FriendHead::Start()
{
	m_vwp_inputManager = GetManager().lock()->GetGameObject("InputManager").lock()->GetGameComponent<InputManager>();
	m_vwp_gameSettings = GetManager().lock()->GetGameObject("GameSettings").lock()->GetGameComponent<GameSettings>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_friendManager = GetManager().lock()->GetGameObject("FriendManager").lock()->GetGameComponent<FriendManager>();

	m_vwp_rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();

	m_vwp_headCenter = GetManager().lock()->AddObjectFromCereal("HeadCenter");
	m_vwp_headCenter.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);

	m_prevPos = Vector3Const::Zero;
	m_crntPos = Vector3Const::Zero;
	m_velocity = Vector3Const::Zero;

	SetPartHitAreaParameter();

	m_vlp_putTimer = ObjectFactory::Create<RelativeTimer>(1);

	m_isPut = false;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendHead::Clone()
{
	auto clone = ObjectFactory::Create<FriendHead>();
	clone->m_trackerIndex = m_trackerIndex;
	return clone;
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
	score += m_vwp_eyesHitAreaComponent.lock()->GetCalcScore();
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

	return m_vwp_noseHitAreaComponent.lock()->GetCalcScore();
}

std::int32_t ButiEngine::FriendHead::GetMouthScore()
{
	auto gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	std::int32_t gameLevel = gameLevelManager->GetGameLevel();

	if (gameLevel == 0)
	{
		return 0;
	}

	return m_vwp_mouthHitAreaComponent.lock()->GetCalcScore();
}

bool ButiEngine::FriendHead::IsBeautiful()
{
	auto gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	std::int32_t gameLevel = gameLevelManager->GetGameLevel();

	if (gameLevel == 0)
	{
		return false;
	}

	std::int32_t dummyPartCount = 0;
	dummyPartCount += m_vwp_eyesHitAreaComponent.lock()->GetDummyPartCount();
	dummyPartCount += m_vwp_noseHitAreaComponent.lock()->GetDummyPartCount();
	dummyPartCount += m_vwp_mouthHitAreaComponent.lock()->GetDummyPartCount();
	
	return dummyPartCount == 0;
}

void ButiEngine::FriendHead::Control()
{
	if (GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize() > m_trackerIndex)
	{
		ControlByVRTracker();
	}

	ControlByGamePad();

	m_vwp_rigidBodyComponent.lock()->TransformApply();
}

void ButiEngine::FriendHead::ControlByGamePad()
{
	Vector3 direction = Vector3Const::Zero;

	Vector2 leftStick = m_vwp_inputManager.lock()->GetLeftStick();
	if (leftStick.GetLengthSqr() != 0.0f)
	{
		direction = leftStick;
	}

	Vector2 rightStick = m_vwp_inputManager.lock()->GetRightStick();
	if (rightStick.GetLength() != 0.0f)
	{
		direction.z = rightStick.y * 0.5f;
	}

	direction.x *= -1.0f;
	direction.z *= -1.0f;
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
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_trackerIndex], deviceMatrix);
	Vector3 pos = deviceMatrix.GetPosition();
	pos *= m_vwp_gameSettings.lock()->GetCorrection();
	pos.x *= -1; 
	auto rotation = deviceMatrix.GetRemovePosition();
	rotation._13 *= -1;
	rotation._31 *= -1;
	rotation._12 *= -1;
	rotation._21 *= -1;
	gameObject.lock()->transform->SetLocalPosition(pos);
	gameObject.lock()->transform->SetLocalRotation(rotation);
}

void ButiEngine::FriendHead::OnPut()
{
	GetManager().lock()->GetGameObject("ScoreManager").lock()->GetGameComponent<ScoreManager>()->CalcScore();

	auto body = GetManager().lock()->GetGameObject(GameObjectTag("FriendBody"));
	body.lock()->GetGameComponent<FriendBody>()->SetHead(gameObject);

	m_vwp_friendManager.lock()->AddFriendCount();

	m_vwp_rigidBodyComponent.lock()->SetIsRemove(true);

	m_isPut = true;
}

void ButiEngine::FriendHead::CalcVelocity()
{
	m_prevPos = m_crntPos;
	m_crntPos = gameObject.lock()->transform->GetLocalPosition();

	m_velocity = m_crntPos - m_prevPos;
}

void ButiEngine::FriendHead::CheckPut()
{
	if (!CanPut())
	{
		return;
	}

	if (GameDevice::GetInput().GetPadButtonTrigger(ButiInput::PadButtons::XBOX_A))
	{
		OnPut();
	}

	if (GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize() <= m_trackerIndex)
	{
		return;
	}

	////‘ä‚É‹ß‚­‚ÄˆÚ“®‘¬“x‚ª’x‚©‚Á‚½‚ç’u‚¢‚½‚Æ”»’è‚·‚é
	//Matrix4x4 deviceMatrix;
	//GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_trackerIndex], deviceMatrix);
	//Vector3 pos = deviceMatrix.GetPosition();
	//Vector3 tablePos = m_vwp_gameSettings.lock()->GetTablePos();

	constexpr float putTeleranceY = 0.05f;
	constexpr float putTeleranceXZ = 0.75f;
	//constexpr float putMoveSpeedBorder = 0.01f;

	//float distanceY = abs(pos.y - tablePos.y);
	//float distanceYSqr = distanceY * distanceY;
	//float putTleranceYSqr = putTeleranceY * putTeleranceY;
	//float distanceXZSqr = (Vector2(pos.x, pos.z) - Vector2(tablePos.x, tablePos.z)).GetLengthSqr();

	//float moveSpeedSqr = abs(m_velocity.GetLengthSqr());
	//float putMoveSpeedBorderSqr = putMoveSpeedBorder * putMoveSpeedBorder;

	//if (distanceYSqr <= putTleranceYSqr && 
	//	distanceXZSqr <= putTeleranceXZ && 
	//	moveSpeedSqr <= putMoveSpeedBorderSqr)
	//{
	//	if (!m_vlp_putTimer->IsOn())
	//	{
	//		m_vlp_putTimer->Start();
	//	}

	//	if (m_vlp_putTimer->Update())
	//	{
	//		OnPut();
	//	}
	//}
	//else
	//{
	//	m_vlp_putTimer->Reset();
	//}

	auto body = GetManager().lock()->GetGameObject(GameObjectTag("FriendBody"));
	if (!body.lock())
	{
		return;
	}

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 bodyPos = body.lock()->transform->GetLocalPosition();

	float distanceSqr = (pos - bodyPos).GetLengthSqr();

	if (distanceSqr <= putTeleranceXZ)
	{
		OnPut();
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

void ButiEngine::FriendHead::SetPartHitAreaParameter()
{
	m_vwp_eyesHitArea = GetManager().lock()->AddObjectFromCereal("PartHitArea_Eyes");
	m_vwp_noseHitArea = GetManager().lock()->AddObjectFromCereal("PartHitArea_Nose");
	m_vwp_mouthHitArea = GetManager().lock()->AddObjectFromCereal("PartHitArea_Mouth");

	m_vwp_eyesHitAreaComponent = m_vwp_eyesHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();
	m_vwp_noseHitAreaComponent = m_vwp_noseHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();
	m_vwp_mouthHitAreaComponent = m_vwp_mouthHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();

	m_vwp_eyesHitArea.lock()->transform->SetBaseTransform(m_vwp_headCenter.lock()->transform, true);
	m_vwp_noseHitArea.lock()->transform->SetBaseTransform(m_vwp_headCenter.lock()->transform, true);
	m_vwp_mouthHitArea.lock()->transform->SetBaseTransform(m_vwp_headCenter.lock()->transform, true);

	m_eyesStandardPos = m_vwp_eyesHitArea.lock()->transform->GetLocalPosition();
	m_noseStandardPos = m_vwp_noseHitArea.lock()->transform->GetLocalPosition();
	m_mouthStandardPos = m_vwp_mouthHitArea.lock()->transform->GetLocalPosition();
}
