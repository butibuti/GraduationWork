#include "stdafx_u.h"
#include "Effect_HeartSpawner.h"

void ButiEngine::Effect_HeartSpawner::OnUpdate()
{
	if (m_vlp_spawnTimer->Update())
	{
		SpawnHeart();
		SetSpawnInterval();
	}
}

void ButiEngine::Effect_HeartSpawner::OnSet()
{
}

void ButiEngine::Effect_HeartSpawner::OnRemove()
{
}

void ButiEngine::Effect_HeartSpawner::OnShowUI()
{
}

void ButiEngine::Effect_HeartSpawner::Start()
{
	m_vlp_spawnTimer = ObjectFactory::Create<RelativeTimer>(60);
	m_vlp_spawnTimer->Start();

	m_minSpawnIntervalFrame = 10;
	m_maxSpawnIntervalFrame = 15;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Effect_HeartSpawner::Clone()
{
	return ObjectFactory::Create<Effect_HeartSpawner>();
}

void ButiEngine::Effect_HeartSpawner::SpawnHeart()
{
	auto heart = GetManager().lock()->AddObjectFromCereal("Effect_Heart");

	Vector3 pos = Vector3(0.0f, -700.0f, 0.1f);
	pos.x = ButiRandom::GetRandom(-1000.0f, 1000.0f, 10);

	heart.lock()->transform->SetLocalPosition(pos);

	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
	scale.x = ButiRandom::GetInt(50, 150);
	scale.y = scale.x;

	heart.lock()->transform->SetLocalScale(scale);
}

void ButiEngine::Effect_HeartSpawner::SetSpawnInterval()
{
	std::int32_t spawnIntervalFrame = ButiRandom::GetInt(m_minSpawnIntervalFrame, m_maxSpawnIntervalFrame);

	m_vlp_spawnTimer->ChangeCountFrame(spawnIntervalFrame);
	m_vlp_spawnTimer->Reset();
}
