#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ScoreManager;

	class UI_Score :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "UI_Score";
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
		Value_weak_ptr<ScoreManager> m_vwp_scoreManager;
		Value_weak_ptr<TextDrawComponent> m_vwp_textDraw;
	};

}

BUTI_REGIST_GAMECOMPONENT(UI_Score, true);