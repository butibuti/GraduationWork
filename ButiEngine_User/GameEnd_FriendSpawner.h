#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class GameEnd_FriendSpawner :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "GameEnd_FriendSpawner";
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
		}

		void SpawnFriends();
	private:
		void SpawnFriends(const std::int32_t arg_spawnCount);
	};

}

BUTI_REGIST_GAMECOMPONENT(GameEnd_FriendSpawner, true);