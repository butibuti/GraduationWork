#include "stdafx_u.h"
#include "FriendHead.h"
#include "InputManager.h"
#include "GameSettings.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "PauseManager.h"
#include "FriendManager.h"
#include "FriendBody.h"
#include "ScoreManager.h"

void ButiEngine::FriendHead::OnUpdate()
{
	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return;
	}

	Control();
	CalcVelocity();
	CheckPut();
	//CheckSpawnBody();

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
}

void ButiEngine::FriendHead::OnShowUI()
{
	GUI::DragInt("TrackerIndex", m_trackerIndex, 1.0, 0, 16);


	GUI::BulletText(U8("左目の基準位置"));
	GUI::DragFloat3("##leftEyeStandardPos", &m_leftEyeStandardPos.x, 1.0f, -10.0f, 10.0f);

	GUI::BulletText(U8("右目の基準位置"));
	GUI::DragFloat3("##rightEyeStandardPos", &m_rightEyeStandardPos.x, 1.0f, -10.0f, 10.0f);

	GUI::BulletText(U8("鼻の基準位置"));
	GUI::DragFloat3("##noseStandardPos", &m_noseStandardPos.x, 1.0f, -10.0f, 10.0f);

	GUI::BulletText(U8("口の基準位置"));
	GUI::DragFloat3("##mouthStandardPos", &m_mouthStandardPos.x, 1.0f, -10.0f, 10.0f);


	GUI::BulletText(U8("目のスコアが0になる距離"));
	GUI::DragFloat("##eyeFurthest", &m_eyeFurthest, 1.0f, -100.0f, 100.0f);

	GUI::BulletText(U8("鼻のスコアが0になる距離"));
	GUI::DragFloat("##noseFurthest", &m_noseFurthest, 1.0f, -100.0f, 100.0f);

	GUI::BulletText(U8("口のスコアが0になる距離"));
	GUI::DragFloat("##mouthFurthest", &m_mouthFurthest, 1.0f, -100.0f, 100.0f);
}

void ButiEngine::FriendHead::Start()
{
	m_vwp_inputManager = GetManager().lock()->GetGameObject("InputManager").lock()->GetGameComponent<InputManager>();
	m_vwp_gameSettings = GetManager().lock()->GetGameObject("GameSettings").lock()->GetGameComponent<GameSettings>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_friendManager = GetManager().lock()->GetGameObject("FriendManager").lock()->GetGameComponent<FriendManager>();

	m_vwp_rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();

	m_prevPos = Vector3Const::Zero;
	m_crntPos = Vector3Const::Zero;
	m_velocity = Vector3Const::Zero;

	m_vec_eyes.clear();
	m_vec_noses.clear();
	m_vec_mouths.clear();

	m_maxEyeCount = 2;
	m_maxNoseCount = 1;
	m_maxMouthCount = 1;

	m_vlp_putTimer = ObjectFactory::Create<RelativeTimer>(10);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendHead::Clone()
{
	auto clone = ObjectFactory::Create<FriendHead>();
	clone->m_trackerIndex = m_trackerIndex;
	clone->m_leftEyeStandardPos = m_leftEyeStandardPos;
	clone->m_rightEyeStandardPos = m_rightEyeStandardPos;
	clone->m_noseStandardPos = m_noseStandardPos;
	clone->m_mouthStandardPos = m_mouthStandardPos;
	clone->m_eyeFurthest = m_eyeFurthest;
	clone->m_noseFurthest = m_noseFurthest;
	clone->m_mouthFurthest = m_mouthFurthest;
	return clone;
}

std::int32_t ButiEngine::FriendHead::GetEyeScore()
{
	std::int32_t score = 0;

	auto tmp_vec_eyes = m_vec_eyes;

	//左目の基準位置に最も近い目を左目とする
	Value_weak_ptr<GameObject> leftEye;
	float leftEyeDistance = m_eyeFurthest * 10000;

	auto end = tmp_vec_eyes.end();
	for (auto itr = tmp_vec_eyes.begin(); itr != end; ++itr)
	{
		Vector3 eyePos = (*itr).lock()->transform->GetLocalPosition();
		float distance = eyePos.Distance(m_leftEyeStandardPos);

		if (distance <= leftEyeDistance)
		{
			leftEyeDistance = distance;
			leftEye = (*itr);
		}
	}

	//左目のスコア加算
	float progress = 1.0f - (leftEyeDistance / m_eyeFurthest);
	progress = MathHelper::Clamp(progress, 0.0f, 1.0f);

	std::int32_t addScore = MathHelper::Lerp(0, 25, progress);
	score += addScore;


	//スコアを計算した目をリストから削除
	auto find = std::find(tmp_vec_eyes.begin(), tmp_vec_eyes.end(), leftEye);
	tmp_vec_eyes.erase(find);


	//右目の基準位置に最も近い目を右目とする
	Value_weak_ptr<GameObject> rightEye;
	float rightEyeDistance = m_eyeFurthest * 10000;

	if (tmp_vec_eyes.size() == 1)
	{
		rightEye = tmp_vec_eyes[0];
		Vector3 eyePos = rightEye.lock()->transform->GetLocalPosition();
		float distance = eyePos.Distance(m_rightEyeStandardPos);
		rightEyeDistance = distance;
	}
	else
	{
		for (auto itr = tmp_vec_eyes.begin(); itr != end; ++itr)
		{
			Vector3 eyePos = (*itr).lock()->transform->GetLocalPosition();
			float distance = eyePos.Distance(m_rightEyeStandardPos);

			if (distance <= rightEyeDistance)
			{
				rightEyeDistance = distance;
				rightEye = (*itr);
			}
		}
	}

	//右目のスコア加算
	progress = 1.0f - (rightEyeDistance / m_eyeFurthest);
	progress = MathHelper::Clamp(progress, 0.0f, 1.0f);

	addScore = MathHelper::Lerp(0, 25, progress);
	score += addScore;

	return score;
}

std::int32_t ButiEngine::FriendHead::GetNoseScore()
{
	std::int32_t score = 0;

	auto end = m_vec_noses.end();
	for (auto itr = m_vec_noses.begin(); itr != end; ++itr)
	{
		Vector3 nosePos = (*itr).lock()->transform->GetLocalPosition();
		float distance = nosePos.Distance(m_noseStandardPos);

		float progress = 1.0f - (distance / m_noseFurthest);
		progress = MathHelper::Clamp(progress, 0.0f, 1.0f);

		std::int32_t addScore = MathHelper::Lerp(0, 25, progress);
		score += addScore;
	}

	return score;
}

std::int32_t ButiEngine::FriendHead::GetMouthScore()
{
	std::int32_t score = 0;

	auto end = m_vec_mouths.end();
	for (auto itr = m_vec_mouths.begin(); itr != end; ++itr)
	{
		Vector3 mouthPos = (*itr).lock()->transform->GetLocalPosition();
		float distance = mouthPos.Distance(m_mouthStandardPos);

		float progress = 1.0f - (distance / m_mouthFurthest);
		progress = MathHelper::Clamp(progress, 0.0f, 1.0f);

		std::int32_t addScore = MathHelper::Lerp(0, 25, progress);
		score += addScore;
	}

	return score;
}

void ButiEngine::FriendHead::Dead()
{
	gameObject.lock()->SetIsRemove(true);
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
	m_vwp_friendManager.lock()->AddFriendCount();

	auto body = GetManager().lock()->GetGameObject(GameObjectTag("FriendBody"));
	body.lock()->GetGameComponent<FriendBody>()->SetHead(gameObject);

	SetIsRemove(true);
	m_vwp_rigidBodyComponent.lock()->SetIsRemove(true);
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("FriendHead"));
}

void ButiEngine::FriendHead::CalcVelocity()
{
	m_prevPos = m_crntPos;
	m_crntPos = gameObject.lock()->transform->GetLocalPosition();

	m_velocity = m_crntPos - m_prevPos;

	//if (IsHighSpeed())
	//{
	//	m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetCollisionGroup(2);
	//}
	//else
	//{
	//	m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetCollisionGroup(1);
	//}
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

	//台に近くて移動速度が遅かったら置いたと判定する
	Matrix4x4 deviceMatrix;
	GameDevice::GetVRTrackerInput().GetDevicePoseMatrix(GameDevice::GetVRTrackerInput().GetAllDeviceNames()[m_trackerIndex], deviceMatrix);
	Vector3 pos = deviceMatrix.GetPosition();
	Vector3 tablePos = m_vwp_gameSettings.lock()->GetTablePos();

	constexpr float putTelerance = 0.1f;
	constexpr float putMoveSpeedBorder = 0.01f;

	float distanceSqr = (pos - tablePos).GetLengthSqr();
	float putTleranceSqr = putTelerance * putTelerance;

	float moveSpeedSqr = abs(m_velocity.GetLengthSqr());
	float putMoveSpeedBorderSqr = putMoveSpeedBorder * putMoveSpeedBorder;

	if (distanceSqr <= putTleranceSqr && moveSpeedSqr <= putMoveSpeedBorderSqr)
	{
		if (!m_vlp_putTimer->IsOn())
		{
			m_vlp_putTimer->Start();
		}

		if (m_vlp_putTimer->Update())
		{
			OnPut();
		}
	}
	else
	{
		m_vlp_putTimer->Reset();
	}
}

bool ButiEngine::FriendHead::CanPut()
{
	constexpr std::int32_t necessaryEyeCount = 2;
	constexpr std::int32_t necessaryNoseCount = 1;
	constexpr std::int32_t necessaryMouthCount = 1;

	if (m_vec_eyes.size() < necessaryEyeCount)
	{
		return false;
	}

	if (m_vec_noses.size() < necessaryNoseCount)
	{
		return false;
	}

	if (m_vec_mouths.size() < necessaryMouthCount)
	{
		return false;
	}

	return true;
}
