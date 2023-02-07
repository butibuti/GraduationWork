#include "stdafx_u.h"
#include "RandomFriendFacePartSpawner.h"
#include "PauseManager.h"
#include "StageManager.h"
#include "GameLevelManager.h"
#include "FriendFacePart.h"

void ButiEngine::RandomFriendFacePartSpawner::OnUpdate()
{
	if (!CanUpdate())
	{
		return;
	}

	if (m_vlp_spawnFacePartTimer->Update())
	{
		SpawnFacePart();
		SetSpawnFacePartInterval();
	}

	if (m_vlp_spawnDummyPartTimer->Update())
	{
		SpawnDummyPart();
		SetSpawnDummyPartInterval();
	}
}

void ButiEngine::RandomFriendFacePartSpawner::OnSet()
{
}

void ButiEngine::RandomFriendFacePartSpawner::OnRemove()
{
}

void ButiEngine::RandomFriendFacePartSpawner::OnShowUI()
{
	GUI::BulletText(U8("ƒp[ƒcoŒ»‚·‚éˆÊ’u"));
	GUI::DragFloat("##SpawnPos", m_spawnPosZ, 0.1f, -100.0f, 100.0f);

	if (!m_vwp_gameLevelManager.lock())
	{
		m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();
	}

	ResizeLevelParameter();

	std::int32_t maxLevel = m_vwp_gameLevelManager.lock()->GetMaxLevel();
	for (std::int32_t i = 1;i < maxLevel + 1;i++)
	{

		if (GUI::TreeNode("Level:" + std::to_string(i)))
		{
			if (GUI::TreeNode("FacePart"))
			{
				GUI::BulletText(U8("¶¬‚·‚éãŒÀ"));
				GUI::DragInt("##MaxFacePartCount" + std::to_string(i), m_vec_maxFacePartCounts[i], 1.0f, 0, 100);

				GUI::BulletText(U8("¶¬‚·‚éŠÔŠu‚Ì‰ºŒÀ"));
				GUI::DragInt("##MinSpawnFacePartIntervalFrame" + std::to_string(i), m_vec_minSpawnFacePartIntervalFrames[i], 1.0f, 0, 600);

				GUI::BulletText(U8("¶¬‚·‚éŠÔŠu‚ÌãŒÀ"));
				GUI::DragInt("##MaxSpawnFacePartIntervalFrame" + std::to_string(i), m_vec_maxSpawnFacePartIntervalFrames[i], 1.0f, 0, 600);

				GUI::TreePop();
			}

			if (GUI::TreeNode("DummyPart"))
			{
				GUI::BulletText(U8("¶¬‚·‚éãŒÀ"));
				GUI::DragInt("##MaxDummyPartCount" + std::to_string(i), m_vec_maxDummyPartCounts[i], 1.0f, 0, 100);

				GUI::BulletText(U8("¶¬‚·‚éŠÔŠu‚Ì‰ºŒÀ"));
				GUI::DragInt("##MinSpawnDummyPartIntervalFrame" + std::to_string(i), m_vec_minSpawnDummyPartIntervalFrames[i], 1.0f, 0, 600);

				GUI::BulletText(U8("¶¬‚·‚éŠÔŠu‚ÌãŒÀ"));
				GUI::DragInt("##MaxSpawnDummyPartIntervalFrame" + std::to_string(i), m_vec_maxSpawnDummyPartIntervalFrames[i], 1.0f, 0, 600);

				GUI::TreePop();
			}

			GUI::TreePop();
		}
	}
}

void ButiEngine::RandomFriendFacePartSpawner::Start()
{
	auto tutorialManager = GetManager().lock()->GetGameObject("TutorialManager");
	if (tutorialManager.lock())
	{
		m_isTutorial = true;
	}

	if (!m_isTutorial)
	{
		m_vwp_stageManager = GetManager().lock()->GetGameObject("StageManager").lock()->GetGameComponent<StageManager>();
	}
	m_vwp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	m_vwp_gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager").lock()->GetGameComponent<GameLevelManager>();

	ResizeLevelParameter();

	FirstSpawnFacePart();

	m_vlp_spawnFacePartTimer = ObjectFactory::Create<RelativeTimer>(60);
	SetSpawnFacePartInterval();
	m_vlp_spawnFacePartTimer->Start();

	m_vlp_spawnDummyPartTimer = ObjectFactory::Create<RelativeTimer>(60);
	SetSpawnDummyPartInterval();
	m_vlp_spawnDummyPartTimer->Start();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::RandomFriendFacePartSpawner::Clone()
{
	auto clone = ObjectFactory::Create<RandomFriendFacePartSpawner>();
	clone->m_spawnPosZ = m_spawnPosZ;

	clone->m_vec_maxFacePartCounts = m_vec_maxFacePartCounts;
	clone->m_vec_minSpawnFacePartIntervalFrames = m_vec_minSpawnFacePartIntervalFrames;
	clone->m_vec_maxSpawnFacePartIntervalFrames = m_vec_maxSpawnFacePartIntervalFrames;

	clone->m_vec_maxDummyPartCounts = m_vec_maxDummyPartCounts;
	clone->m_vec_minSpawnDummyPartIntervalFrames = m_vec_minSpawnDummyPartIntervalFrames;
	clone->m_vec_maxSpawnDummyPartIntervalFrames = m_vec_maxSpawnDummyPartIntervalFrames;
	return clone;
}

void ButiEngine::RandomFriendFacePartSpawner::FirstSpawnFacePart()
{
	auto eye = GetManager().lock()->AddObjectFromCereal("FriendFacePart_Eyes");
	eye.lock()->transform->SetLocalPosition(GetRandomSpawnPartPos(false, true));

	auto nose = GetManager().lock()->AddObjectFromCereal("FriendFacePart_Nose");
	nose.lock()->transform->SetLocalPosition(GetRandomSpawnPartPos(false, true));

	auto mouth = GetManager().lock()->AddObjectFromCereal("FriendFacePart_Mouth");
	mouth.lock()->transform->SetLocalPosition(GetRandomSpawnPartPos(false, true));
}

void ButiEngine::RandomFriendFacePartSpawner::SpawnFacePart()
{
 	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	std::int32_t maxFacePartCount = m_vec_maxFacePartCounts[gameLevel];

	std::int32_t maxCount = maxFacePartCount;
	std::int32_t eyeSpawnCount = maxCount - FriendFacePart::GetEyeCount();
	for (std::int32_t i = 0; i < eyeSpawnCount; i++)
	{
		auto eye = GetManager().lock()->AddObjectFromCereal("FriendFacePart_Eyes");
		eye.lock()->transform->SetLocalPosition(GetRandomSpawnPartPos(false));
		auto eyeComponent = eye.lock()->GetGameComponent<FriendFacePart>();
		//eyeComponent->SetRandomParam_Straight();
	}

	std::int32_t noseSpawnCount = maxCount - FriendFacePart::GetNoseCount();
	for (std::int32_t i = 0; i < noseSpawnCount; i++)
	{
		auto nose = GetManager().lock()->AddObjectFromCereal("FriendFacePart_Nose");
		nose.lock()->transform->SetLocalPosition(GetRandomSpawnPartPos(false));
		auto noseComponent = nose.lock()->GetGameComponent<FriendFacePart>();
		//noseComponent->SetRandomParam_Straight();
	}

	std::int32_t mouthSpawnCount = maxCount - FriendFacePart::GetMouthCount();
	for (std::int32_t i = 0; i < mouthSpawnCount; i++)
	{
		auto mouth = GetManager().lock()->AddObjectFromCereal("FriendFacePart_Mouth");
		mouth.lock()->transform->SetLocalPosition(GetRandomSpawnPartPos(false));
		auto mouthComponent = mouth.lock()->GetGameComponent<FriendFacePart>();
		//mouthComponent->SetRandomParam_Straight();
	}
}

void ButiEngine::RandomFriendFacePartSpawner::SetSpawnFacePartInterval()
{
	if (!m_vlp_spawnFacePartTimer)
	{
		return;
	}
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	std::int32_t spawnIntervalFrame = ButiRandom::GetInt(m_vec_minSpawnFacePartIntervalFrames[gameLevel], m_vec_maxSpawnFacePartIntervalFrames[gameLevel]);
	spawnIntervalFrame = max(spawnIntervalFrame, 0);

	m_vlp_spawnFacePartTimer->ChangeCountFrame(spawnIntervalFrame);
	m_vlp_spawnFacePartTimer->Reset();
}

void ButiEngine::RandomFriendFacePartSpawner::SpawnDummyPart()
{
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	if (FriendFacePart::GetDummyPartCount() >= m_vec_maxDummyPartCounts[gameLevel])
	{
		return;
	}

	auto dummyPart = GetManager().lock()->AddObjectFromCereal("FriendFacePart_Dummy");
	dummyPart.lock()->transform->SetLocalPosition(GetRandomSpawnPartPos());
	auto dummyPartComponent = dummyPart.lock()->GetGameComponent<FriendFacePart>();
	dummyPartComponent->SetRandomParam_Straight();
}

void ButiEngine::RandomFriendFacePartSpawner::SetSpawnDummyPartInterval()
{
	if (!m_vlp_spawnDummyPartTimer)
	{
		return;
	}
	std::int32_t gameLevel = m_vwp_gameLevelManager.lock()->GetGameLevel();
	std::int32_t spawnIntervalFrame = ButiRandom::GetInt(m_vec_minSpawnDummyPartIntervalFrames[gameLevel], m_vec_maxSpawnDummyPartIntervalFrames[gameLevel]);
	spawnIntervalFrame = max(spawnIntervalFrame, 0);

	m_vlp_spawnDummyPartTimer->ChangeCountFrame(spawnIntervalFrame);
	m_vlp_spawnDummyPartTimer->Reset();
}

ButiEngine::Vector3 ButiEngine::RandomFriendFacePartSpawner::GetRandomSpawnPartPos(bool arg_isStraight, bool arg_isFirstSpawn)
{
	std::string movePatternStr = "Stay";
	if (arg_isFirstSpawn)
	{
		movePatternStr = "Stay_First";
	}
	else if (arg_isStraight)
	{
		movePatternStr = "Straight";
	}

	std::string tagName = "FriendFacePartSpawnArea_" + movePatternStr;
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

std::string ButiEngine::RandomFriendFacePartSpawner::GetRandomSpawnPartName()
{
	std::string facePartName;

	std::int32_t maxRandom = 2;

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

	return facePartName;
}

bool ButiEngine::RandomFriendFacePartSpawner::CanUpdate()
{
	if (m_isTutorial)
	{
		return false;
	}
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

void ButiEngine::RandomFriendFacePartSpawner::ResizeLevelParameter()
{
	std::int32_t maxLevel = m_vwp_gameLevelManager.lock()->GetMaxLevel();
	if (m_vec_maxFacePartCounts.size() != (maxLevel + 1))
	{
		m_vec_maxFacePartCounts.resize(maxLevel + 1);
		m_vec_maxDummyPartCounts.resize(maxLevel + 1);
	}

	if (m_vec_minSpawnFacePartIntervalFrames.size() != (maxLevel + 1))
	{
		m_vec_minSpawnFacePartIntervalFrames.resize(maxLevel + 1);
		m_vec_minSpawnDummyPartIntervalFrames.resize(maxLevel + 1);
	}

	if (m_vec_maxSpawnFacePartIntervalFrames.size() != (maxLevel + 1))
	{
		m_vec_maxSpawnFacePartIntervalFrames.resize(maxLevel + 1);
		m_vec_maxSpawnDummyPartIntervalFrames.resize(maxLevel + 1);
	}
}