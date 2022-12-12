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
		}

		std::int32_t GetFriendCount() { return m_friendCount; }

		void ResetCurrentLevelFriendCount() { m_currentLevelFriendCount = 0; }
		void AddFriendCount();

		static void ClearFriendData() { g_vec_friendDatas.clear(); }
		static void AddFriendData(Value_ptr<FriendData> arg_data) { g_vec_friendDatas.push_back(arg_data); }
	private:
		void SpawnFriends();

		Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;

		static std::vector<Value_ptr<FriendData>> g_vec_friendDatas;

		std::int32_t m_friendCount;
		std::int32_t m_currentLevelFriendCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendManager, true);