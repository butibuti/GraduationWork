#include "stdafx_u.h"
#include "Bomb.h"
#include "BlowFriend.h"
#include "GameCamera.h"
#include "FriendBodySpawner.h"
#include "FriendManager.h"
#include "FriendHead.h"
#include "NumberDraw.h"
#include "FriendBody.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include "SpriteParticleGenerator.h"

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
		m_vlp_explodeTimer->SetCount(m_vlp_explodeTimer->GetMaxCountFrame());
		m_vlp_waitExplodeTimer->Start();

		m_vwp_timerTextComponent.lock()->SetNumber(0);
	}

	if (m_vlp_explodeTimer->IsOn())
	{
		SetTimer();
		TinderAnimation();
	}

	BombAnimation();

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
	m_vwp_particleGenerator = GetManager().lock()->GetGameObject("SpriteParticleGenerator").lock()->GetGameComponent<SpriteParticleGenerator>();

	m_vwp_timerText = GetManager().lock()->AddObjectFromCereal("BombTimerText");
	m_vwp_timerText.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	m_vwp_timerTextComponent = m_vwp_timerText.lock()->GetGameComponent<NumberDraw>();
	m_vwp_timerTextComponent.lock()->SetMeshDrawComponent();

	m_vlp_explodeTimer = ObjectFactory::Create<RelativeTimer>(m_frameToExplode);
	m_vlp_waitExplodeTimer = ObjectFactory::Create<RelativeTimer>(120);
	m_vlp_explodeTimer->Start();

	m_vwp_meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	m_vwp_tinderTransform = gameObject.lock()->GetGameComponent<ModelDrawComponent>()->GetTransform();
	m_vwp_spawnSparkTransform = gameObject.lock()->GetGameComponent<ModelDrawComponent>()->GetBone()->searchBoneByName("Bone")->transform;

	m_theta = 0.0f;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Bomb::Clone()
{
	return ObjectFactory::Create<Bomb>();
}

void ButiEngine::Bomb::Dead()
{
	if (m_vwp_timerText.lock())
	{
		m_vwp_timerText.lock()->SetIsRemove(true);
	}

	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::Bomb::StartFall()
{
	m_isFall = true;
	m_vlp_explodeTimer->Stop();
}

void ButiEngine::Bomb::Explode()
{
	auto explosion= AddObjectFromCereal("Explosion");
	explosion.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
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

		GetManager().lock()->GetGameObject("FriendManager").lock()->GetGameComponent<FriendManager>()->RemoveCompleteFriend(randomIndex);
	}

	GetManager().lock()->GetGameObject("FriendBodySpawner").lock()->GetGameComponent<FriendBodySpawner>()->DecreaceBodiesNumber();

	m_vwp_parent.lock()->GetGameComponent<FriendBody>()->Blow();

	Dead();
}

void ButiEngine::Bomb::BombAnimation()
{
	float progress = m_vlp_explodeTimer->GetPercent();
	float animSpeed = MathHelper::Lerp(0.03f, 0.5f, progress);
	m_theta += animSpeed;

	float animProgress = sin(m_theta);
	//ColorAnimation(animProgress);
	//ScaleAnimation(animProgress);
}

void ButiEngine::Bomb::ColorAnimation(const float arg_progress)
{
	Vector3 newColor = MathHelper::LerpPosition(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.5f, 0.0f, 0.0f), arg_progress);
	m_vwp_meshDraw.lock()->SetColor(Vector4(newColor, 1.0f));
}

void ButiEngine::Bomb::ScaleAnimation(const float arg_progress)
{
	Vector3 newScale = MathHelper::LerpPosition(Vector3(1.0f, 1.0f, 1.0f), Vector3(1.05f, 1.05f, 1.05f), arg_progress);
	gameObject.lock()->GetTransform()->SetLocalScale(newScale);
}

void ButiEngine::Bomb::Fall()
{
	Vector3 pos = gameObject.lock()->transform->TranslateY(-0.15f);

	if (pos.y < -30.0f)
	{
		Dead();
	}
}

void ButiEngine::Bomb::SetTimer()
{
	float remainSecond = m_vlp_explodeTimer->GetRemainFrame() / 60.0f;
	std::int32_t number = static_cast<std::int32_t>(remainSecond * 100);
	m_vwp_timerTextComponent.lock()->SetNumber(number);
}

void ButiEngine::Bomb::TinderAnimation()
{
	Vector3 newScale = m_vwp_tinderTransform.lock()->GetLocalScale();
	float progress = m_vlp_explodeTimer->GetPercent();
	float newScaleY = MathHelper::Lerp(1.0f, 0.0f, progress);
	newScale.y = newScaleY;
	m_vwp_tinderTransform.lock()->SetLocalScale(newScale);

	SpawnSparkEffect();
}

void ButiEngine::Bomb::SpawnSparkEffect()
{
	Vector3 screenPos = m_vwp_spawnSparkTransform.lock()->GetWorldPosition();// GetCamera("main")->WorldToScreen(m_vwp_spawnSparkTransform.lock()->GetWorldPosition());
	//screenPos.z = 0.0f;
	m_vwp_particleGenerator.lock()->Spark(Vector3Const::Zero, m_vwp_spawnSparkTransform);
}
