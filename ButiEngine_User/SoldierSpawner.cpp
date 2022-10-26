#include "stdafx_u.h"
#include "SoldierSpawner.h"

void ButiEngine::SoldierSpawner::OnUpdate()
{
	if (m_vlp_spawnIntervalTimer->Update())
	{
		SpawnSoldier();
	}
}

void ButiEngine::SoldierSpawner::OnSet()
{
}

void ButiEngine::SoldierSpawner::OnRemove()
{
}

void ButiEngine::SoldierSpawner::OnShowUI()
{
	GUI::BulletText(U8("¶¬‚·‚éãŒÀ"));
	GUI::DragInt("##MaxSoldierCount", m_maxSoldierCount, 1.0f, 0, 100);

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

void ButiEngine::SoldierSpawner::Start()
{
	m_vlp_spawnIntervalTimer = ObjectFactory::Create<RelativeTimer>(m_spawnIntervalFrame);
	m_vlp_spawnIntervalTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SoldierSpawner::Clone()
{
	auto clone = ObjectFactory::Create<SoldierSpawner>();
	clone->m_maxSoldierCount = m_maxSoldierCount;
	clone->m_spawnIntervalFrame = m_spawnIntervalFrame;
	return clone;
}

void ButiEngine::SoldierSpawner::Dead()
{
}

void ButiEngine::SoldierSpawner::SpawnSoldier()
{
	auto soldiers = GetManager().lock()->GetGameObjects(GameObjectTag("Soldier"));
	if (soldiers.GetSize() > m_maxSoldierCount)
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

	auto soldier = GetManager().lock()->AddObjectFromCereal("Soldier");
	soldier.lock()->transform->SetLocalPosition(spawnPos);
}
