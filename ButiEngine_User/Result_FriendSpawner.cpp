#include "stdafx_u.h"
#include "Result_FriendSpawner.h"
#include "FriendManager.h"
#include "Result_CompleteFriend.h"
#include "CompleteFriend.h"
#include "FriendFacePart.h"

void ButiEngine::Result_FriendSpawner::OnUpdate()
{
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::Q))
	{
		SpawnFriends(8);
	}
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::W))
	{
		SpawnFriends(13);
	}
	if (GameDevice::GetInput().TriggerKey(ButiInput::Keys::E))
	{
		SpawnFriends(20);
	}
}

void ButiEngine::Result_FriendSpawner::OnSet()
{
	if (!m_vlp_friendSpawnArea)
	{
		m_vlp_friendSpawnArea = ObjectFactory::Create<FriendSpawnArea>();
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
		GUI::BulletText(U8("LeftPos(¶‚ª+‰E‚ª-)"));
		GUI::DragFloat("##LeftPos", m_vlp_friendSpawnArea->leftPos, 0.1f, -3000.0f, 3000.0f);

		GUI::BulletText(U8("RightPos(¶‚ª+‰E‚ª-)"));
		GUI::DragFloat("##RightPos", m_vlp_friendSpawnArea->rightPos, 0.1f, -3000.0f, 3000.0f);

		GUI::BulletText("Space");
		GUI::DragFloat("##Space", m_vlp_friendSpawnArea->space, 1.0f, 0.1f, 100.0f);

		GUI::TreePop();
	}
}

void ButiEngine::Result_FriendSpawner::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Result_FriendSpawner::Clone()
{
	auto clone = ObjectFactory::Create<Result_FriendSpawner>();
	clone->m_vlp_friendSpawnArea = m_vlp_friendSpawnArea;
	return clone;
}

ButiEngine::Vector3 ButiEngine::Result_FriendSpawner::GetFriendSpawnPos(const std::int32_t arg_friendNum)
{
	constexpr float friendScaleX = 2.0f;
	std::int32_t spawnAreaIndex = 0;

	float spawnStartPos = m_vlp_friendSpawnArea->leftPos;
	Vector3 spawnPos = Vector3(spawnStartPos, 30.0f, 0.0f);
	spawnPos.x -= m_vlp_friendSpawnArea->space * arg_friendNum;

	if (arg_friendNum % 2 == 0)
	{
		spawnPos.z -= 2.0f;
	}

	return spawnPos;
}

void ButiEngine::Result_FriendSpawner::SpawnFriends()
{
	std::int32_t friendDataIndex = 0;

	auto friendDatas = FriendManager::GetFriendDatas();

	auto friendDataEnd = friendDatas.end();
	for (auto friendDataItr = friendDatas.begin(); friendDataItr != friendDataEnd; ++friendDataItr)
	{
		auto completeFriend = GetManager().lock()->AddObjectFromCereal("Result_CompleteFriend");
		completeFriend.lock()->GetGameComponent<CompleteFriend>()->CreateParts((*friendDataItr));

		completeFriend.lock()->transform->SetLocalPosition(GetFriendSpawnPos(friendDataIndex));

		friendDataIndex++;
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
		data->eyeRank = Rank::Normal;
		data->noseRank = Rank::Normal;
		data->mouthRank = Rank::Normal;

		friendDatas.push_back(data);
	}

	SpawnFriends();
}
