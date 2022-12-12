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

void ButiEngine::FriendHead::OnUpdate()
{
	if (!CanUpdate())
	{
		return;
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
		m_vwp_eyesHitArea.lock()->SetIsRemove(true);
	}
	if (m_vwp_noseHitArea.lock())
	{
		m_vwp_noseHitArea.lock()->SetIsRemove(true);
	}
	if (m_vwp_mouthHitArea.lock())
	{
		m_vwp_mouthHitArea.lock()->SetIsRemove(true);
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
	m_vwp_headCenterComponent = m_vwp_headCenter.lock()->GetGameComponent<FriendHead_Center>();

	m_prevPos = Vector3Const::Zero;
	m_crntPos = Vector3Const::Zero;
	m_velocity = Vector3Const::Zero;

	SetPartHitAreaParameter();

	m_vlp_putTimer = ObjectFactory::Create<RelativeTimer>(1);

	m_isPut = false;

#ifdef DEBUG
	gameObject.lock()->transform->SetLocalPosition(Vector3Const::Zero);
	m_vwp_rigidBodyComponent.lock()->TransformApply();
#endif // DEBUG
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

std::vector<ButiEngine::Value_weak_ptr<ButiEngine::GameObject>> ButiEngine::FriendHead::GetDummies()
{
	std::vector<ButiEngine::Value_weak_ptr<ButiEngine::GameObject>> vec_dummies;
	
	auto eyesHitAreaDummyParts = m_vwp_eyesHitAreaComponent.lock()->GetStickDummyParts();
	auto noseHitAreaDummyParts = m_vwp_noseHitAreaComponent.lock()->GetStickDummyParts();
	auto mouthHitAreaDummyParts = m_vwp_mouthHitAreaComponent.lock()->GetStickDummyParts();

	vec_dummies.insert(vec_dummies.end(), eyesHitAreaDummyParts.begin(), eyesHitAreaDummyParts.end());
	vec_dummies.insert(vec_dummies.end(), noseHitAreaDummyParts.begin(), noseHitAreaDummyParts.end());
	vec_dummies.insert(vec_dummies.end(), mouthHitAreaDummyParts.begin(), mouthHitAreaDummyParts.end());

	return vec_dummies;
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

void ButiEngine::FriendHead::OnPut(Value_weak_ptr<GameObject> arg_vwp_body)
{
	GetManager().lock()->GetGameObject("ScoreManager").lock()->GetGameComponent<ScoreManager>()->CalcScore(gameObject, arg_vwp_body);

	arg_vwp_body.lock()->GetGameComponent<FriendBody>()->SetHead(gameObject);

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

	//if (GameDevice::GetInput().GetPadButtonTrigger(ButiInput::PadButtons::XBOX_A))
	//{
	//	OnPut();
	//}

	//if (GameDevice::GetVRTrackerInput().GetAllDeviceNames().GetSize() <= m_trackerIndex)
	//{
	//	return;
	//}

	////��ɋ߂��Ĉړ����x���x��������u�����Ɣ��肷��
	//Matrix4x4 deviceMatrix;
	//GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_trackerIndex], deviceMatrix);
	//Vector3 pos = deviceMatrix.GetPosition();
	//Vector3 tablePos = m_vwp_gameSettings.lock()->GetTablePos();

	//constexpr float putTeleranceY = 0.05f;
	//constexpr float putTeleranceXZ = 0.75f;
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

	auto collisionBody = m_vwp_headCenterComponent.lock()->GetCollisionFriendBody();
	if (collisionBody.lock())
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

void ButiEngine::FriendHead::SetPartHitAreaParameter()
{
	m_vwp_eyesHitArea = GetManager().lock()->AddObjectFromCereal("PartHitArea_Eyes");
	m_vwp_noseHitArea = GetManager().lock()->AddObjectFromCereal("PartHitArea_Nose");
	m_vwp_mouthHitArea = GetManager().lock()->AddObjectFromCereal("PartHitArea_Mouth");

	m_vwp_eyesHitAreaComponent = m_vwp_eyesHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();
	m_vwp_noseHitAreaComponent = m_vwp_noseHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();
	m_vwp_mouthHitAreaComponent = m_vwp_mouthHitArea.lock()->GetGameComponent<FriendHead_PartHitArea>();

	m_vwp_eyesHitAreaComponent.lock()->SetParent(m_vwp_headCenter);
	m_vwp_noseHitAreaComponent.lock()->SetParent(m_vwp_headCenter);
	m_vwp_mouthHitAreaComponent.lock()->SetParent(m_vwp_headCenter);
}
