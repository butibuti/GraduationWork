#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class GameEndManager :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "GameEndManager";
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
		Value_ptr<RelativeTimer> m_vlp_gameEndTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(GameEndManager, true);