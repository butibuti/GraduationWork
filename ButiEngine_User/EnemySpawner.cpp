#include "stdafx_u.h"
#include "EnemySpawner.h"

void ButiEngine::EnemySpawner::OnUpdate()
{
	if (m_vlp_spawnIntervalTimer->Update())
	{
		SpawnEnemy();
	}
}

void ButiEngine::EnemySpawner::OnSet()
{
}

void ButiEngine::EnemySpawner::OnRemove()
{
}

void ButiEngine::EnemySpawner::OnShowUI()
{
	GUI::BulletText(U8("¶¬‚·‚éãŒÀ"));
	GUI::DragInt("##MaxEnemyCount", m_maxEnemyCount, 1.0f, 0, 100);

	GUI::BulletText(U8("¶¬‚·‚éŠÔŠu"));
	if (GUI::DragInt("##SpawnIntervalFrame", m_spawnIntervalFrame, 1.0f, 0, 600))
	{
		if (m_vlp_spawnIntervalTimer)
		{
			m_vlp_spawnIntervalTimer->ChangeCountFrame(m_spawnIntervalFrame);
			m_vlp_spawnIntervalTimer->Reset();
		}
	}
}

void ButiEngine::EnemySpawner::Start()
{
	m_vlp_spawnIntervalTimer = ObjectFactory::Create<RelativeTimer>(m_spawnIntervalFrame);
	m_vlp_spawnIntervalTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EnemySpawner::Clone()
{
	auto clone = ObjectFactory::Create<EnemySpawner>();
	clone->m_maxEnemyCount = m_maxEnemyCount;
	clone->m_spawnIntervalFrame = m_spawnIntervalFrame;
	return clone;
}

void ButiEngine::EnemySpawner::Dead()
{
}

void ButiEngine::EnemySpawner::SpawnEnemy()
{
	auto soldiers = GetManager().lock()->GetGameObjects(GameObjectTag("Enemy"));
	if (soldiers.GetSize() > m_maxEnemyCount)
	{
		return;
	}

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 scale = gameObject.lock()->transform->GetLocalScale();
	Vector3 minPos = pos - scale * 0.5f;
	Vector3 maxPos = pos + scale * 0.5f;

	Vector3 spawnPos;
	spawnPos.x = ButiRandom::GetRandom(minPos.x, maxPos.x, 10);
	spawnPos.y = 0.5f;
	spawnPos.z = ButiRandom::GetRandom(minPos.z, maxPos.z, 10);

	auto soldier = GetManager().lock()->AddObjectFromCereal("Enemy_Test");
	soldier.lock()->transform->SetLocalPosition(spawnPos);
}
