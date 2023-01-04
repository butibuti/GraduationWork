#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	struct FriendData
	{
		Value_ptr<Transform> vlp_headTransform;
		Value_ptr<Transform> vlp_eyeTransform;
		Value_ptr<Transform> vlp_noseTransform;
		Value_ptr<Transform> vlp_mouthTransform;
		std::vector<Value_ptr<Transform>> vec_vlp_dummyTransforms;
		Value_ptr<Transform> vlp_bodyTransform;
	};

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

	class GameLevelManager;

	class FriendManager :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FriendManager";
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
			ARCHIVE_BUTI(m_vec_friendSpawnAreas);
		}

		std::int32_t GetFriendDataSize() { return m_vec_friendSpawnAreas.size(); }

		void ResetCurrentLevelFriendCount() { m_currentLevelFriendCount = 0; }
		void AddFriendCount();
		void SpawnFriends();

		static void ClearFriendData() { g_vec_friendDatas.clear(); }
		static void AddFriendData(Value_ptr<FriendData> arg_data) { g_vec_friendDatas.push_back(arg_data); }
	private:
		void SpawnFriends(const std::int32_t arg_spawnCount);

		Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;

		std::int32_t m_friendCount;
		std::int32_t m_currentLevelFriendCount;

		static std::vector<Value_ptr<FriendData>> g_vec_friendDatas;
		std::vector<Value_ptr<FriendSpawnArea>> m_vec_friendSpawnAreas;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendManager, true);