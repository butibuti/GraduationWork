#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	struct FriendSpawnArea
	{
		float leftPos;
		float rightPos;
		std::int32_t maxFriendCount;

		template<class Archive>
		void serialize(Archive& archive)
		{
			ARCHIVE_BUTI(leftPos);
			ARCHIVE_BUTI(rightPos);
			ARCHIVE_BUTI(maxFriendCount);
		}
	};

	class Result_FriendSpawner :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Result_FriendSpawner";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnRemove()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			ARCHIVE_BUTI(isActive);
			ARCHIVE_BUTI(m_vlp_friendSpawnArea);
		}

		Vector3 GetFriendSpawnPos(const std::int32_t arg_friendNum);
		void SpawnFriends();
	private:
		void SpawnFriends(const std::int32_t arg_spawnCount);

		Value_ptr<FriendSpawnArea> m_vlp_friendSpawnArea;
	};

}

BUTI_REGIST_GAMECOMPONENT(Result_FriendSpawner, true);