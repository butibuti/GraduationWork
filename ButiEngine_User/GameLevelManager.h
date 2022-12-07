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
			ARCHIVE_BUTI(m_maxLevel);
			ARCHIVE_BUTI(m_vec_necessaryFriendCounts);
			ARCHIVE_BUTI(m_vec_levelUpIntervalFrames);
		}

		std::int32_t GetGameLevel() { return m_gameLevel; }
		std::int32_t GetMaxLevel() { return m_maxLevel; }

		void ChangeLevelUpInterval(const std::int32_t arg_levelUpIntervalFrame);

		void CheckLevelUp(const std::int32_t arg_currentLevelFriendCount);
		void LevelUp();
	private:
		void ChangeLevel(const std::int32_t arg_nextLevel);
		void ResizeLevelParameter();

		std::int32_t m_gameLevel;
		std::int32_t m_maxLevel;
		std::vector<std::int32_t> m_vec_necessaryFriendCounts;
		std::vector<std::int32_t> m_vec_levelUpIntervalFrames;

		bool m_isLevelLock;

		bool m_isLevelUp;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<FriendManager> m_vwp_friendManager;

		Value_ptr<RelativeTimer> m_vlp_levelUpIntervalTimer;

		Value_ptr<RelativeTimer> m_vlp_levelUpTimer;
		Value_ptr<RelativeTimer> m_vlp_pauseTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(GameLevelManager, true);