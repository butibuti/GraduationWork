#include "stdafx_u.h"
#include "FriendManager.h"
#include "GameLevelManager.h"
#include "Result_CompleteFriend.h"

#ifdef DEBUG
#include "Header/GameObjects/DefaultGameComponent/TriggerComponent.h"
#include "FriendHead_PartHitArea.h"
#endif // DEBUG

std::vector<ButiEngine::Value_ptr<ButiEngine::FriendData>> ButiEngine::FriendManager::g_vec_friendDatas;

void ButiEngine::FriendManager::OnUpdate()
{
}

void ButiEngine::FriendManager::OnSet()
{
	if (m_vec_friendSpawnAreas.size() == 0)
	{
		m_vec_friendSpawnAreas.push_back(ObjectFactory::Create<FriendSpawnArea>());
	}
}

void ButiEngine::FriendManager::OnRemove()
{
}

void ButiEngine::FriendManager::OnShowUI()
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	std::string sceneName = sceneManager->GetCurrentScene()->GetSceneInformation()->GetSceneName();
	if (sceneName != "Result")
	{
		return;
	}

	GUI::Text("FriendDataCount:" + std::to_string(g_vec_friendDatas.size()));
	if (GUI::Button("ClearFriendData"))
	{
		ClearFriendData();
	}

	if (GUI::Button("Spawn"))
	{
		SpawnFriends();
	}

	static std::int32_t spawnCount;
	GUI::DragInt("SpawnCount", spawnCount, 1.0f, 0, 1000);
	if (GUI::Button("SpawnSpecify"))
	{
		SpawnFriends(spawnCount);
	}


	if (GUI::TreeNode("SpawnArea"))
	{
		if (GUI::Button("AddSpawnArea"))
		{
			m_vec_friendSpawnAreas.push_back(ObjectFactory::Create<FriendSpawnArea>());
		}

		if (m_vec_friendSpawnAreas.size() >= 2)
		{
			GUI::SameLine();

			if (GUI::Button("RemoveSpawnArea"))
			{
				m_vec_friendSpawnAreas.pop_back();
			}
		}

		std::int32_t index = 0;
		auto end = m_vec_friendSpawnAreas.end();
		for (auto itr = m_vec_friendSpawnAreas.begin(); itr != end; ++itr)
		{
			if (GUI::TreeNode("SpawnArea" + std::to_string(index)))
			{
				GUI::BulletText(U8("LeftPos(¶‚ª+‰E‚ª-)"));
				GUI::DragFloat("##LeftPos" + std::to_string(index), (*itr)->leftPos, 0.1f, -3000.0f, 3000.0f);

				GUI::BulletText(U8("RightPos(¶‚ª+‰E‚ª-)"));
				GUI::DragFloat("##RightPos" + std::to_string(index), (*itr)->rightPos, 0.1f, -3000.0f, 3000.0f);

				GUI::BulletText("MaxFriendCount");
				GUI::DragInt("##MaxFriendCount" + std::to_string(index), (*itr)->maxFriendCount, 1.0f, 1, 100);

				GUI::TreePop();
			}

			index++;
		}

		GUI::TreePop();
	}
}

void ButiEngine::FriendManager::Start()
{
	auto gameLevelManager = GetManager().lock()->GetGameObject("GameLevelManager");
	if (gameLevelManager.lock())
	{
		m_vwp_gameLevelManager = gameLevelManager.lock()->GetGameComponent<GameLevelManager>();
	}

	m_friendCount = 0;
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FriendManager::Clone()
{
	auto clone = ObjectFactory::Create<FriendManager>();
	clone->m_vec_friendSpawnAreas = m_vec_friendSpawnAreas;
	return clone;
}

void ButiEngine::FriendManager::AddFriendCount()
{
	m_friendCount++;
	m_currentLevelFriendCount++;

	m_vwp_gameLevelManager.lock()->CheckLevelUp(m_currentLevelFriendCount);
}

void ButiEngine::FriendManager::SpawnFriends()
{
	constexpr float friendScaleX = 2.0f;
	std::int32_t friendDataIndex = 0;
	std::int32_t spawnAreaIndex = 0;
	
	auto friendDataEnd = g_vec_friendDatas.end();
	for (auto friendDataItr = g_vec_friendDatas.begin(); friendDataItr != friendDataEnd; ++friendDataItr)
	{
		float spawnAreaLength = abs(m_vec_friendSpawnAreas[spawnAreaIndex]->rightPos - m_vec_friendSpawnAreas[spawnAreaIndex]->leftPos) - friendScaleX;
		std::int32_t maxFriendCount = max(m_vec_friendSpawnAreas[spawnAreaIndex]->maxFriendCount - 1, 1);
		float space = spawnAreaLength / maxFriendCount;
		float spawnStartPos = m_vec_friendSpawnAreas[spawnAreaIndex]->leftPos - friendScaleX * 0.5f;
		Vector3 spawnPos = Vector3(spawnStartPos, 30.0f, 0.0f);
		spawnPos.x -= space * friendDataIndex;

		auto completeFriend = GetManager().lock()->AddObjectFromCereal("Result_CompleteFriend");
		completeFriend.lock()->GetGameComponent<Result_CompleteFriend>()->CreateParts((*friendDataItr));

		completeFriend.lock()->transform->SetLocalPosition(spawnPos);

		friendDataIndex++;

		if (friendDataIndex >= m_vec_friendSpawnAreas[spawnAreaIndex]->maxFriendCount)
		{
			if (m_vec_friendSpawnAreas.size() > spawnAreaIndex + 1)
			{
				spawnAreaIndex++;
				friendDataIndex = 0;
			}
		}
	}
}

void ButiEngine::FriendManager::SpawnFriends(const std::int32_t arg_spawnCount)
{
	auto eyesHitArea = GetManager().lock()->GetGameObject("Eyes_Default");
	auto noseHitArea = GetManager().lock()->GetGameObject("Nose_Default");
	auto mouthHitArea = GetManager().lock()->GetGameObject("Mouth_Default");

	while (g_vec_friendDatas.size() < arg_spawnCount)
	{
		auto data = ObjectFactory::Create<FriendData>();
		data->vlp_bodyTransform = ObjectFactory::Create<Transform>();
		data->vlp_headTransform = ObjectFactory::Create<Transform>();
		data->vlp_eyeTransform = eyesHitArea.lock()->transform->Clone();
		data->vlp_noseTransform = noseHitArea.lock()->transform->Clone();
		data->vlp_mouthTransform = mouthHitArea.lock()->transform->Clone();

		g_vec_friendDatas.push_back(data);
	}

	SpawnFriends();
}
