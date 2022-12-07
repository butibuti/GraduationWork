#include "stdafx_u.h"
#include "FriendBody.h"
#include "PauseManager.h"
#include "GameSettings.h"
#include "FriendCompleteDirecting.h"
#include "GameLevelManager.h"
#include "FriendBody_Neck.h"

void ButiEngine::FriendBody::OnUpdate()
{
	if (m_vwp_pauseManager.lock()->IsPause())
	{
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
	if (!m_vwp_gameLevelManager.lock())
	{
		m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	}

	std::int32_t maxLevel = m_vwp_gameLevelManager.lock()->GetMaxLevel();
	if (m_vec_rotateSpeeds.size() != (maxLevel + 1))
	{
		m_vec_rotateSpeeds.resize(maxLevel + 1);
	}

	if (m_vec_moveHorizontalFrame.size() != (maxLevel + 1))
	{
		m_vec_moveHorizontalFrame.resize(maxLevel + 1);
	}

	GUI::BulletText("FrontBorder");
	GUI::DragFloat("##frontBorder", &m_frontBorder, 0.1f, 0.0f, 180.0f);

	for (std::int32_t i = 1; i < maxLevel + 1; i++)
	{
		GUI::Text("Level:" + std::to_string(i));

		GUI::BulletText("RotateSpeed");
		GUI::DragFloat("##rotateSpeed" + std::to_string(i), &m_vec_rotateSpeeds[i], 0.1f, 0.0f, 1000.0f);

		GUI::BulletText("MoveFrame");
		GUI::DragInt("##moveFrame" + std::to_string(i), &m_vec_moveHorizontalFrame[i], 1.0f, 1.0f, 1000.0f);
	}
}

void ButiEngine::FriendBody::Start()
{
	m_vwp_gameSettings = GetManager().lock()->GetGameObject("GameSettings").lock()->GetGameComponent<GameSettings>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();

	m_vwp_neck = GetManager().lock()->AddObjectFromCereal("FriendBody_Neck");
	m_vwp_neck.lock()->SetObjectName(gameObject.lock()->GetGameObjectName() + "_Neck");
	m_vwp_neck.lock()->GetGameComponent<FriendBody_Neck>()->SetParent(gameObject);

	m_isRotate = true;
	m_isStopRotate = false;

	m_isMoveBack = false;
	m_vlp_moveBackTimer = ObjectFactory::Create<RelativeTimer>(90);
	m_moveBackStartPos = Vector3Const::Zero;
	m_moveBackTargetPos = Vector3Const::Zero;

	m_isTurned = false;
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
	arg_vwp_head.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	arg_vwp_head.lock()->transform->SetLocalPosition(Vector3Const::Zero);

	m_isMoveHorizontal = false;

	gameObject.lock()->AddGameComponent<FriendCompleteDirecting>();
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("FriendBody"));
	
	if (m_vwp_neck.lock())
	{
		m_vwp_neck.lock()->SetIsRemove(true);
	}

	StartMoveBack();
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

bool ButiEngine::FriendBody::IsFast()
{
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	if (gameLevel == 0)
	{
		return false;
	}

	if (m_isTurned)
	{
		return false;
	}

	return gameObject.lock()->transform->GetLocalPosition().x > 0.0f;
}

void ButiEngine::FriendBody::Rotate()
{
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	if (gameLevel == 0)
	{
		return;
	}

	gameObject.lock()->transform->RollLocalRotationY_Degrees(m_vec_rotateSpeeds[gameLevel] * GameDevice::GetWorldSpeed());

	if (m_isStopRotate)
	{
		if (IsFront())
		{
			m_isRotate = false;
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
		SpawnNewBody();
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
		m_isTurned = true;
	}
}

void ButiEngine::FriendBody::SpawnNewHead()
{
	GetManager().lock()->AddObjectFromCereal("FriendHead");
}

void ButiEngine::FriendBody::SpawnNewBody()
{
	GetManager().lock()->AddObjectFromCereal("FriendBody");
}
