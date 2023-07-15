#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	enum class Rank;

	struct FriendData
	{
		Value_ptr<Transform> vlp_headTransform;
		Value_ptr<Transform> vlp_eyeTransform;
		Value_ptr<Transform> vlp_noseTransform;
		Value_ptr<Transform> vlp_mouthTransform;
		Value_ptr<Transform> vlp_bodyTransform;

		Rank eyeRank;
		Rank noseRank;
		Rank mouthRank;
		Rank totalRank;

		bool hasHelmet;
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

		void ResetCurrentLevelFriendCount() { m_currentLevelFriendCount = 0; }
		void AddFriendCount();

		static std::vector<Value_weak_ptr<GameObject>>& GetCompleteFriends() { return g_vec_completeFriends; }
		static std::vector<Value_ptr<FriendData>>& GetFriendDatas() { return g_vec_friendDatas; }
		static std::int32_t GetCompleteFriendTargetPositionIndex();
		static void AddCompleteFriendTargetPositionIndex(const std::uint16_t arg_index) { g_vec_targetPosIndexs.push_back(arg_index); }

		static void ClearFriendData()
		{
			g_vec_completeFriends.clear();
			g_vec_friendDatas.clear(); 
		}

		static void ResetTargetPosIndexs()
		{
			g_vec_targetPosIndexs.clear();
			for (std::uint16_t i = 0; i < 30; i++)
			{
				g_vec_targetPosIndexs.push_back(i);
			}
		}

		void AddCompleteFriend(Value_weak_ptr<GameObject> arg_friend, Value_ptr<FriendData> arg_data);
		void RemoveCompleteFriend(const std::int32_t arg_index);
		void RemoveCompleteFriend(Value_weak_ptr<GameObject> arg_friend);
	private:
		Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;

		std::int32_t m_friendCount;
		std::int32_t m_currentLevelFriendCount;

		static std::vector<Value_weak_ptr<GameObject>> g_vec_completeFriends;
		static std::vector<Value_ptr<FriendData>> g_vec_friendDatas;
		static std::vector<std::uint16_t> g_vec_targetPosIndexs;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendManager, true);