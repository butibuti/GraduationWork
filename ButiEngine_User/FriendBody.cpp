#include "stdafx_u.h"
#include "FriendBody.h"
#include "PauseManager.h"
#include "GameSettings.h"
#include "FriendCompleteDirecting.h"

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
}

void ButiEngine::FriendBody::Start()
{
	m_vwp_gameSettings = GetManager().lock()->GetGameObject("GameSettings").lock()->GetGameComponent<GameSettings>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	//gameObject.lock()->transform->SetLocalPosition(m_vwp_gameSettings.lock()->GetBodyPos());

	m_rotateSpeed = 8.0f;

	m_isRotate = true;
	m_isStopRotate = false;
	m_isMoveBack = false;

	m_vlp_moveTimer = ObjectFactory::Create<RelativeTimer>(90);
	m_moveTargetPos = Vector3Const::Zero;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendBody::Clone()
{
	auto clone = ObjectFactory::Create<FriendBody>();
	clone->m_frontBorder = m_frontBorder;
	return clone;
}

void ButiEngine::FriendBody::SetHead(Value_weak_ptr<GameObject> arg_vwp_head)
{
	arg_vwp_head.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	arg_vwp_head.lock()->transform->SetLocalPosition(Vector3Const::Zero);

	gameObject.lock()->AddGameComponent<FriendCompleteDirecting>();
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("FriendBody"));

	StartMoveBack();
}

std::int32_t ButiEngine::FriendBody::GetScore()
{
	//デフォルトは1
	std::int32_t score = 1;

	//正面を向いていたらスコア倍
	float rotation = gameObject.lock()->transform->GetLocalRotation_Euler().y;

	if (IsFront())
	{
		score = 2;
	}

	return score;
}

void ButiEngine::FriendBody::Rotate()
{
	gameObject.lock()->transform->RollLocalRotationY_Degrees(m_rotateSpeed * GameDevice::GetWorldSpeed());

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
	float progress = m_vlp_moveTimer->GetPercent();
	Vector3 newPos = MathHelper::LerpPosition(m_moveStartPos, m_moveTargetPos, progress);

	gameObject.lock()->transform->SetLocalPosition(newPos);

	if (m_vlp_moveTimer->Update())
	{
		m_vlp_moveTimer->Stop();
		m_isMoveBack = false;
		m_isStopRotate = true;

		gameObject.lock()->transform->SetLocalPosition(m_moveTargetPos);

		SpawnNewHead();
		SpawnNewBody();
	}
}

void ButiEngine::FriendBody::StartMoveBack()
{
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();

	m_moveStartPos = pos;

	m_moveTargetPos.x = pos.x + (ButiRandom::GetInt(-5, 5));
	m_moveTargetPos.y = pos.y;
	m_moveTargetPos.z = pos.z + (ButiRandom::GetInt(-10, -5));

	m_isMoveBack = true;
	m_vlp_moveTimer->Start();
}

void ButiEngine::FriendBody::SpawnNewHead()
{
	GetManager().lock()->AddObjectFromCereal("FriendHead");
}

void ButiEngine::FriendBody::SpawnNewBody()
{
	GetManager().lock()->AddObjectFromCereal("FriendBody");
}

bool ButiEngine::FriendBody::IsFront()
{
	Vector3 front = gameObject.lock()->transform->GetFront();
	float angle = abs(MathHelper::ToDegree(std::acos(front.Dot(Vector3Const::ZAxis))));

	return angle <= m_frontBorder;
}
