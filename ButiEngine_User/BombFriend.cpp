#include "stdafx_u.h"
#include "BombFriend.h"
#include "Bomb.h"
#include "FriendBody.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include "PauseManager.h"

void ButiEngine::BombFriend::OnUpdate()
{
	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return;
	}
	if (m_vlp_explodeTimer->Update())
	{
		m_vlp_explodeTimer->Stop();

		auto friendBody = gameObject.lock()->GetGameComponent<FriendBody>();
		friendBody->RemoveNeck();
		friendBody->StartMoveBack(Vector3(0, 0, -40), 60);
	}
}

void ButiEngine::BombFriend::OnSet()
{
}

void ButiEngine::BombFriend::OnRemove()
{
}

void ButiEngine::BombFriend::OnShowUI()
{
}

void ButiEngine::BombFriend::Start()
{
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	m_vlp_explodeTimer = ObjectFactory::Create<RelativeTimer>(m_frameToExplode);
	m_vlp_explodeTimer->Start();

	CreateBomb();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BombFriend::Clone()
{
	return ObjectFactory::Create<BombFriend>();
}

void ButiEngine::BombFriend::StopTimer()
{
	if (m_vwp_bomb.lock())
	{
		m_vwp_bomb.lock()->GetGameComponent<Bomb>()->StartFall();
	}

	m_vlp_explodeTimer->Stop();
	SetIsRemove(true);
}

void ButiEngine::BombFriend::Dead()
{
	if (m_vwp_bomb.lock())
	{
		m_vwp_bomb.lock()->GetGameComponent<Bomb>()->Dead();
	}

	auto friendBody = gameObject.lock()->GetGameComponent<FriendBody>();
	friendBody->RemoveGuideHead();
	friendBody->RemoveHeart();

	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::BombFriend::CreateBomb()
{
	auto modelDraw = gameObject.lock()->GetGameComponent<ModelDrawComponent>();
	auto bone = modelDraw->GetBone()->searchBoneByName("Bomb");
	m_vwp_bomb = GetManager().lock()->AddObjectFromCereal("Bomb");
	m_vwp_bomb.lock()->transform->SetLocalPosition(Vector3Const::Zero);
	m_vwp_bomb.lock()->transform->SetBaseTransform(bone->transform, true);

	auto bomb = m_vwp_bomb.lock()->GetGameComponent<Bomb>();
	bomb->SetFrameToExplode(m_frameToExplode);
	bomb->SetParent(gameObject);
}
