#include "stdafx_u.h"
#include "Result_FriendSpawner.h"
#include "FriendManager.h"
#include "Result_CompleteFriend.h"

void ButiEngine::Result_FriendSpawner::OnUpdate()
{
}

void ButiEngine::Result_FriendSpawner::OnSet()
{
	if (m_vec_friendSpawnAreas.size() == 0)
	{
		m_vec_friendSpawnAreas.push_back(ObjectFactory::Create<FriendSpawnArea>());
	}
}

void ButiEngine::Result_FriendSpawner::OnRemove()
{
}

void ButiEngine::Result_FriendSpawner::OnShowUI()
{
	auto friendDatas = FriendManager::GetFriendDatas();
	GUI::Text("FriendDataCount:" + std::to_string(friendDatas.size()));
	if (GUI::Button("ClearFriendData"))
	{
		FriendManager::ClearFriendData();
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

void ButiEngine::Result_FriendSpawner::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Result_FriendSpawner::Clone()
{
	auto clone = ObjectFactory::Create<Result_FriendSpawner>();
	clone->m_vec_friendSpawnAreas = m_vec_friendSpawnAreas;
	return clone;
}

void ButiEngine::Result_FriendSpawner::SpawnFriends()
{
	constexpr float friendScaleX = 2.0f;
	std::int32_t friendDataIndex = 0;
	std::int32_t spawnAreaIndex = 0;

	auto friendDatas = FriendManager::GetFriendDatas();

	auto friendDataEnd = friendDatas.end();
	for (auto friendDataItr = friendDatas.begin(); friendDataItr != friendDataEnd; ++friendDataItr)
	{
		float spawnAreaLength = abs(m_vec_friendSpawnAreas[spawnAreaIndex]->rightPos - m_vec_friendSpawnAreas[spawnAreaIndex]->leftPos) - friendScaleX;
		std::int32_t maxFriendCount = max(m_vec_friendSpawnAreas[spawnAreaIndex]->maxFriendCount - 2, 1);
		float space = spawnAreaLength / maxFriendCount;
		float spawnStartPos = m_vec_friendSpawnAreas[spawnAreaIndex]->leftPos;
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

void ButiEngine::Result_FriendSpawner::SpawnFriends(const std::int32_t arg_spawnCount)
{
	auto eyesHitArea = GetManager().lock()->GetGameObject("Eyes_Default");
	auto noseHitArea = GetManager().lock()->GetGameObject("Nose_Default");
	auto mouthHitArea = GetManager().lock()->GetGameObject("Mouth_Default");

	auto& friendDatas = FriendManager::GetFriendDatas();

	while (friendDatas.size() < arg_spawnCount)
	{
		auto data = ObjectFactory::Create<FriendData>();
		data->vlp_bodyTransform = ObjectFactory::Create<Transform>();
		data->vlp_headTransform = ObjectFactory::Create<Transform>();
		data->vlp_eyeTransform = eyesHitArea.lock()->transform->Clone();
		data->vlp_noseTransform = noseHitArea.lock()->transform->Clone();
		data->vlp_mouthTransform = mouthHitArea.lock()->transform->Clone();

		friendDatas.push_back(data);
	}

	SpawnFriends();
}
