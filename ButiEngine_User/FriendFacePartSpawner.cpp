#include "stdafx_u.h"
#include "FriendFacePartSpawner.h"

void ButiEngine::FriendFacePartSpawner::OnUpdate()
{
	if (m_vlp_spawnTimer->Update())
	{
		SpawnFacePart();
		SetSpawnInterval();
	}
}

void ButiEngine::FriendFacePartSpawner::OnSet()
{
}

void ButiEngine::FriendFacePartSpawner::OnRemove()
{
}

void ButiEngine::FriendFacePartSpawner::OnShowUI()
{
	GUI::BulletText(U8("ê∂ê¨Ç∑ÇÈè„å¿"));
	GUI::DragInt("##MaxFacePartCount", m_maxFacePartCount, 1.0f, 0, 100);

	GUI::BulletText(U8("ê∂ê¨Ç∑ÇÈä‘äuÇÃâ∫å¿"));
	if (GUI::DragInt("##MinSpawnIntervalFrame", m_minSpawnIntervalFrame, 1.0f, 0, 600))
	{
		SetSpawnInterval();
	}

	GUI::BulletText(U8("ê∂ê¨Ç∑ÇÈä‘äuÇÃè„å¿"));
	if (GUI::DragInt("##MaxSpawnIntervalFrame", m_maxSpawnIntervalFrame, 1.0f, 0, 600))
	{
		SetSpawnInterval();
	}
}

void ButiEngine::FriendFacePartSpawner::Start()
{
	m_vlp_spawnTimer = ObjectFactory::Create<RelativeTimer>(60);
	SetSpawnInterval();
	m_vlp_spawnTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendFacePartSpawner::Clone()
{
	auto clone = ObjectFactory::Create<FriendFacePartSpawner>();
	clone->m_maxFacePartCount = m_maxFacePartCount;
	clone->m_minSpawnIntervalFrame = m_minSpawnIntervalFrame;
	clone->m_maxSpawnIntervalFrame = m_maxSpawnIntervalFrame;
	return clone;
}

void ButiEngine::FriendFacePartSpawner::SpawnFacePart()
{
	auto faceParts = GetManager().lock()->GetGameObjects(GameObjectTag("FriendFacePart"));
	if (faceParts.GetSize() >= m_maxFacePartCount)
	{
		return;
	}

	auto spawnAreas = GetManager().lock()->GetGameObjects(GameObjectTag("FriendFacePartSpawnArea"));
	if (spawnAreas.GetSize() == 0)
	{
		return;
	}

	auto spawnArea = spawnAreas[ButiRandom::GetInt(0, spawnAreas.GetSize() - 1)];

	Vector3 pos = spawnArea->transform->GetLocalPosition();
	Vector3 scale = spawnArea->transform->GetLocalScale();
	Vector3 minPos = pos - scale * 0.5f;
	Vector3 maxPos = pos + scale * 0.5f;

	Vector3 spawnPos;
	spawnPos.x = ButiRandom::GetRandom(minPos.x, maxPos.x, 10);
	spawnPos.y = ButiRandom::GetRandom(minPos.y, maxPos.y, 10);
	spawnPos.z = -2.0f;

	std::string facePartName;
	std::int32_t random = ButiRandom::GetInt(0, 2);
	if (random == 0)
	{
		facePartName = "FriendFacePart_Eye";
	}
	else if (random == 1)
	{
		facePartName = "FriendFacePart_Nose";
	}
	else if (random == 2)
	{
		facePartName = "FriendFacePart_Mouth";
	}

	auto facePart = GetManager().lock()->AddObjectFromCereal(facePartName);
	facePart.lock()->transform->SetLocalPosition(spawnPos);
}

void ButiEngine::FriendFacePartSpawner::SetSpawnInterval()
{
	if (!m_vlp_spawnTimer)
	{
		return;
	}
	std::int32_t spawnIntervalFrame = ButiRandom::GetInt(m_minSpawnIntervalFrame, m_maxSpawnIntervalFrame);
	spawnIntervalFrame = max(spawnIntervalFrame, 0);

	m_vlp_spawnTimer->ChangeCountFrame(spawnIntervalFrame);
	m_vlp_spawnTimer->Reset();
}
