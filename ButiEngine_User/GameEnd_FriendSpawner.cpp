#include "stdafx_u.h"
#include "GameEnd_FriendSpawner.h"
#include "FriendManager.h"
#include "FriendAligner.h"
#include "CompleteFriend.h"
#include "FriendFacePart.h"

void ButiEngine::GameEnd_FriendSpawner::OnUpdate()
{
}

void ButiEngine::GameEnd_FriendSpawner::OnSet()
{
}

void ButiEngine::GameEnd_FriendSpawner::OnRemove()
{
}

void ButiEngine::GameEnd_FriendSpawner::OnShowUI()
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
}

void ButiEngine::GameEnd_FriendSpawner::Start()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameEnd_FriendSpawner::Clone()
{
	return ObjectFactory::Create<GameEnd_FriendSpawner>();
}

void ButiEngine::GameEnd_FriendSpawner::SpawnFriends()
{
	auto friendAligner = GetManager().lock()->GetGameObject("FriendAligner").lock()->GetGameComponent<FriendAligner>();

	auto friendDatas = FriendManager::GetFriendDatas();

	std::int32_t friendDataIndex = 0;
	auto friendDataEnd = friendDatas.end();
	for (auto friendDataItr = friendDatas.begin(); friendDataItr != friendDataEnd; ++friendDataItr)
	{
		auto completeFriend = GetManager().lock()->AddObjectFromCereal("GameEnd_CompleteFriend");
		auto completeFriendComponent = completeFriend.lock()->GetGameComponent<CompleteFriend>();
		completeFriendComponent->CreateParts((*friendDataItr));
		completeFriendComponent->StartDance();

		completeFriend.lock()->transform->SetLocalPosition(friendAligner->GetCalcFriendPos(friendDataIndex));

		friendDataIndex++;
	}
}

void ButiEngine::GameEnd_FriendSpawner::SpawnFriends(const std::int32_t arg_spawnCount)
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
