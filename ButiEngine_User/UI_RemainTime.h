#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class GameTimer;

	class UI_RemainTime :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "UI_RemainTime";
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
		Value_weak_ptr<GameTimer> m_vwp_gameTimer;
		Value_weak_ptr<TextDrawComponent> m_vwp_textDraw;
	};

}

BUTI_REGIST_GAMECOMPONENT(UI_RemainTime, true);