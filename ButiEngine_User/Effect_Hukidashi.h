#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Effect_Hukidashi :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Effect_Hukidashi";
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

		Value_ptr<RelativeTimer> m_vlp_lifeTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(Effect_Hukidashi, true);