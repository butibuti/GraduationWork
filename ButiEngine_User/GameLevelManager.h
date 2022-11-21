#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;
	class FriendManager;

	class GameLevelManager :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "GameLevelManager";
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
			ARCHIVE_BUTI(m_vec_necessaryFriendCounts);
		}

		std::int32_t GetGameLevel() { return m_gameLevel; }

		void CheckLevelUp(const std::int32_t arg_currentLevelFriendCount);
		void LevelUp();
	private:
		std::int32_t m_gameLevel;
		std::vector<std::int32_t> m_vec_necessaryFriendCounts;


		bool m_isLevelUp;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<FriendManager> m_vwp_friendManager;
		Value_ptr<RelativeTimer> m_vlp_levelUpTimer;
		Value_ptr<RelativeTimer> m_vlp_pauseTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(GameLevelManager, true);