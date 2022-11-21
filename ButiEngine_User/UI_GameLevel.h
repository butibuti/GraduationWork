#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class GameLevelManager;

	class UI_GameLevel :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "UI_GameLevel";
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
	private:
		Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;
		Value_weak_ptr<TextDrawComponent> m_vwp_textDraw;
	};

}

BUTI_REGIST_GAMECOMPONENT(UI_GameLevel, true);