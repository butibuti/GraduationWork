#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

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
	private:
		Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;

		std::int32_t m_friendCount;
		std::int32_t m_currentLevelFriendCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendManager, true);