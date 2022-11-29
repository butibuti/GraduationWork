#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;
	class GameTimer;
	class GameLevelManager;

	class StageManager :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "StageManager";
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

		bool IsGameStart() { return m_isGameStart; }
		bool IsGameFinish() { return m_isGameFinish; }
	private:
		void StartGame();
		void ResetGame();

		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<GameTimer> m_vwp_gameTimer;
		Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;

		Value_ptr<RelativeTimer> m_vlp_waitPlayBGMTimer;

		bool m_isGameStart;
		bool m_isGameFinish;
	};

}

BUTI_REGIST_GAMECOMPONENT(StageManager, true);