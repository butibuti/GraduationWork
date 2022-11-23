#include "stdafx_u.h"
#include "FriendBody.h"
#include "PauseManager.h"
#include "GameSettings.h"

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
}

void ButiEngine::FriendBody::OnSet()
{
}

void ButiEngine::FriendBody::OnRemove()
{
}

void ButiEngine::FriendBody::OnShowUI()
{
	GUI::BulletText("ScoreUpBorder");
	GUI::DragFloat("##scoreUpBorder", &m_scoreUpBorder, 0.1f, 0.0f, 180.0f);
}

void ButiEngine::FriendBody::Start()
{
	m_vwp_gameSettings = GetManager().lock()->GetGameObject("GameSettings").lock()->GetGameComponent<GameSettings>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	m_isRotate = true;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendBody::Clone()
{
	auto clone = ObjectFactory::Create<FriendBody>();
	clone->m_scoreUpBorder = m_scoreUpBorder;
	return clone;
}

void ButiEngine::FriendBody::SetHead(Value_weak_ptr<GameObject> arg_vwp_head)
{
	arg_vwp_head.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	arg_vwp_head.lock()->transform->SetLocalPosition(Vector3Const::Zero);
	m_isRotate = false;
	gameObject.lock()->RemoveGameObjectTag(GameObjectTag("FriendBody"));

	gameObject.lock()->transform->TranslateX(ButiRandom::GetInt(-5, 5));
	gameObject.lock()->transform->TranslateY(ButiRandom::GetInt(-2, 2));
	gameObject.lock()->transform->TranslateZ(ButiRandom::GetInt(-10, -5));

	SpawnNewHead();
	SpawnNewBody();
}

std::int32_t ButiEngine::FriendBody::GetScore()
{
	std::int32_t score = 1;

	float rotation = gameObject.lock()->transform->GetLocalRotation_Euler().y;

	if (abs(rotation) <= m_scoreUpBorder)
	{
		score = 2;
	}

	return score;
}

void ButiEngine::FriendBody::Rotate()
{
	gameObject.lock()->transform->RollLocalRotationY_Degrees(2.0f);
}

void ButiEngine::FriendBody::SpawnNewHead()
{
	GetManager().lock()->AddObjectFromCereal("FriendHead");
}

void ButiEngine::FriendBody::SpawnNewBody()
{
	auto newBody = GetManager().lock()->AddObjectFromCereal("FriendBody");
	newBody.lock()->transform->SetLocalPosition(m_vwp_gameSettings.lock()->GetBodyPos());
}
