#include "stdafx_u.h"
#include "Bomb.h"
#include "BlowFriend.h"
#include "GameCamera.h"
#include "FriendBodySpawner.h"
#include "FriendManager.h"
#include "FriendHead.h"

void ButiEngine::Bomb::OnUpdate()
{
	if (m_isFall)
	{
		Fall();
		return;
	}
	if (m_vlp_explodeTimer->Update())
	{
		m_vlp_explodeTimer->Stop();
		m_vlp_waitExplodeTimer->Start();
	}

	if (m_vlp_waitExplodeTimer->Update())
	{
		m_vlp_waitExplodeTimer->Stop();
		Explode();
	}
}

void ButiEngine::Bomb::OnSet()
{
}

void ButiEngine::Bomb::OnRemove()
{
}

void ButiEngine::Bomb::OnShowUI()
{
}

void ButiEngine::Bomb::Start()
{
	m_vlp_explodeTimer = ObjectFactory::Create<RelativeTimer>(m_frameToExplode);
	m_vlp_waitExplodeTimer = ObjectFactory::Create<RelativeTimer>(120);

	m_vlp_explodeTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Bomb::Clone()
{
	return ObjectFactory::Create<Bomb>();
}

void ButiEngine::Bomb::Dead()
{
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::Bomb::StartFall()
{
	m_isFall = true;
	m_vlp_explodeTimer->Stop();
}

void ButiEngine::Bomb::Explode()
{
	GetManager().lock()->GetGameObject("CameraParent").lock()->GetGameComponent<GameCamera>()->StartShake(20, 0.2f);

	for (std::int32_t i = 0; i < 2; i++)
	{
		auto completeFriends = FriendManager::GetCompleteFriends();
		if (completeFriends.size() == 0)
		{
			break;
		}
		std::int32_t randomIndex = ButiRandom::GetInt(0, completeFriends.size() - 1);
		auto blowFriend = completeFriends[randomIndex];
		blowFriend.lock()->AddGameComponent<BlowFriend>();
		blowFriend.lock()->RemoveGameObjectTag(GameObjectTag("CompleteFriend"));

		FriendManager::RemoveCompleteFriend(randomIndex);
	}

	GetManager().lock()->GetGameObject("FriendBodySpawner").lock()->GetGameComponent<FriendBodySpawner>()->DecreaceBodiesNumber();

	m_vwp_parent.lock()->AddGameComponent<BlowFriend>();
	SetIsRemove(true);
}

void ButiEngine::Bomb::ChangeColor()
{
}

void ButiEngine::Bomb::ScaleAnimation()
{
}

void ButiEngine::Bomb::Fall()
{
	Vector3 pos = gameObject.lock()->transform->TranslateY(-0.15f);

	if (pos.y < -30.0f)
	{
		Dead();
	}
}
