#include "stdafx_u.h"
#include "FriendFacePartSpawner.h"
#include "PauseManager.h"
#include "StageManager.h"
#include "GameLevelManager.h"
#include "FriendFacePart.h"

void ButiEngine::FriendFacePartSpawner::OnUpdate()
{
	if (!CanUpdate())
	{
		return;
	}

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
	GUI::BulletText(U8("パーツ出現する位置"));
	GUI::DragFloat("##SpawnPos", m_spawnPosZ, 0.1f, -100.0f, 100.0f);

	if (!m_vwp_gameLevelManager.lock())
	{
		m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	}

	std::int32_t maxLevel = m_vwp_gameLevelManager.lock()->GetMaxLevel();
	if (m_vec_maxFacePartCounts.size() != (maxLevel + 1))
	{
		m_vec_maxFacePartCounts.resize(maxLevel + 1);
	}

	if (m_vec_minSpawnIntervalFrames.size() != (maxLevel + 1))
	{
		m_vec_minSpawnIntervalFrames.resize(maxLevel + 1);
	}

	if (m_vec_maxSpawnIntervalFrames.size() != (maxLevel + 1))
	{
		m_vec_maxSpawnIntervalFrames.resize(maxLevel + 1);
	}

	for (std::int32_t i = 1;i < maxLevel + 1;i++)
	{
		GUI::Text("Level:" + std::to_string(i));

		GUI::BulletText(U8("生成する上限"));
		GUI::DragInt("##MaxFacePartCount" + std::to_string(i), m_vec_maxFacePartCounts[i], 1.0f, 0, 100);

		GUI::BulletText(U8("生成する間隔の下限"));
		GUI::DragInt("##MinSpawnIntervalFrame" + std::to_string(i), m_vec_minSpawnIntervalFrames[i], 1.0f, 0, 600);

		GUI::BulletText(U8("生成する間隔の上限"));
		GUI::DragInt("##MaxSpawnIntervalFrame" + std::to_string(i), m_vec_maxSpawnIntervalFrames[i], 1.0f, 0, 600);
	}
}

void ButiEngine::FriendFacePartSpawner::Start()
{
	m_vwp_stageManager = GetManager().lock()->GetGameObject("StageManager").lock()->GetGameComponent<StageManager>();
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();

	FirstSpawnFacePart();

	m_vlp_spawnTimer = ObjectFactory::Create<RelativeTimer>(60);
	SetSpawnInterval();
	m_vlp_spawnTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendFacePartSpawner::Clone()
{
	auto clone = ObjectFactory::Create<FriendFacePartSpawner>();
	clone->m_vec_maxFacePartCounts = m_vec_maxFacePartCounts;
	clone->m_vec_minSpawnIntervalFrames = m_vec_minSpawnIntervalFrames;
	clone->m_vec_maxSpawnIntervalFrames = m_vec_maxSpawnIntervalFrames;
	clone->m_spawnPosZ = m_spawnPosZ;
	return clone;
}

void ButiEngine::FriendFacePartSpawner::FirstSpawnFacePart()
{
	auto eye = GetManager().lock()->AddObjectFromCereal("FriendFacePart_Eyes");
	eye.lock()->transform->SetLocalPosition(GetRandomSpawnPartPos());
	eye.lock()->GetGameComponent<FriendFacePart>()->SetMovePattern(MovePattern::Stay);

	auto nose = GetManager().lock()->AddObjectFromCereal("FriendFacePart_Nose");
	nose.lock()->transform->SetLocalPosition(GetRandomSpawnPartPos());
	nose.lock()->GetGameComponent<FriendFacePart>()->SetMovePattern(MovePattern::Stay);

	auto mouth = GetManager().lock()->AddObjectFromCereal("FriendFacePart_Mouth");
	mouth.lock()->transform->SetLocalPosition(GetRandomSpawnPartPos());
	mouth.lock()->GetGameComponent<FriendFacePart>()->SetMovePattern(MovePattern::Stay);
}

void ButiEngine::FriendFacePartSpawner::SpawnFacePart()
{
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	auto faceParts = GetManager().lock()->GetGameObjects(GameObjectTag("FriendFacePart"));
	if (faceParts.GetSize() >= m_vec_maxFacePartCounts[gameLevel])
	{
		return;
	}

	auto facePart = GetManager().lock()->AddObjectFromCereal(GetRandomSpawnPartName());
	facePart.lock()->transform->SetLocalPosition(GetRandomSpawnPartPos());
	
	auto facePartComponent = facePart.lock()->GetGameComponent<FriendFacePart>();

	if (gameLevel <= 1)
	{
		facePartComponent->SetMovePattern(MovePattern::Stay);
	}
	else if (gameLevel <= 2)
	{
		facePartComponent->SetMovePattern(MovePattern::Straight);
	}
	else if (gameLevel <= 3)
	{
		facePartComponent->SetMovePattern(MovePattern::Throw);
	}
}

void ButiEngine::FriendFacePartSpawner::SetSpawnInterval()
{
	if (!m_vlp_spawnTimer)
	{
		return;
	}
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	std::int32_t spawnIntervalFrame = ButiRandom::GetInt(m_vec_minSpawnIntervalFrames[gameLevel], m_vec_maxSpawnIntervalFrames[gameLevel]);
	spawnIntervalFrame = max(spawnIntervalFrame, 0);

	m_vlp_spawnTimer->ChangeCountFrame(spawnIntervalFrame);
	m_vlp_spawnTimer->Reset();
}

ButiEngine::Vector3 ButiEngine::FriendFacePartSpawner::GetRandomSpawnPartPos()
{
	std::string tagName = "FriendFacePartSpawnArea_" + std::to_string(m_vwp_gameLevelManager.lock()->GetGameLevel());
	auto spawnAreas = GetManager().lock()->GetGameObjects(GameObjectTag(tagName));
	if (spawnAreas.GetSize() == 0)
	{
		return Vector3(0.0f, 0.0f, m_spawnPosZ);
	}

	auto spawnArea = spawnAreas[ButiRandom::GetInt(0, spawnAreas.GetSize() - 1)];

	Vector3 pos = spawnArea->transform->GetLocalPosition();
	Vector3 scale = spawnArea->transform->GetLocalScale();
	Vector3 minPos = pos - scale * 0.5f;
	Vector3 maxPos = pos + scale * 0.5f;

	Vector3 spawnPos;
	spawnPos.x = ButiRandom::GetRandom(minPos.x, maxPos.x, 10);
	spawnPos.y = ButiRandom::GetRandom(minPos.y, maxPos.y, 10);
	spawnPos.z = m_spawnPosZ;

	return spawnPos;
}

std::string ButiEngine::FriendFacePartSpawner::GetRandomSpawnPartName()
{
	std::string facePartName;

	std::int32_t maxRandom = 2;
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	if (gameLevel >= 3)
	{
		maxRandom = 3;
	}

	std::int32_t random = ButiRandom::GetInt(0, maxRandom);
	if (random <= 0)
	{
		facePartName = "FriendFacePart_Eyes";
	}
	else if (random <= 1)
	{
		facePartName = "FriendFacePart_Nose";
	}
	else if (random <= 2)
	{
		facePartName = "FriendFacePart_Mouth";
	}
	else if (random <= 3)
	{
		facePartName = "FriendFacePart_Dummy";
	}

	return facePartName;
}

bool ButiEngine::FriendFacePartSpawner::CanUpdate()
{
	if (!m_vwp_stageManager.lock()->IsGameStart())
	{
		return false;
	}

	if (m_vwp_pauseManager.lock()->IsPause())
	{
		return false;
	}

	return true;
}