#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class UI_FriendCount :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "UI_FriendCount";
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
		void SetText();

		Value_weak_ptr<TextDrawComponent> m_vwp_textDraw;
	};

}

BUTI_REGIST_GAMECOMPONENT(UI_FriendCount, true);