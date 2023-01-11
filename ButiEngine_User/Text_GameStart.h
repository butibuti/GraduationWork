#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpriteAnimationComponent;

	class Text_GameStart :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Text_GameStart";
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
			ARCHIVE_BUTI(m_life);
			ARCHIVE_BUTI(m_animIntervalFrame);
		}
	private:
		void Animation();
		void AddAnimation();

		Value_ptr<RelativeTimer> m_vlp_lifeTimer;

		Value_weak_ptr<SpriteAnimationComponent> m_vwp_spriteAnimation;
		Value_ptr<RelativeTimer> m_vlp_animationIntervalTimer;

		std::int32_t m_life;
		std::int32_t m_animIntervalFrame;
	};

}

BUTI_REGIST_GAMECOMPONENT(Text_GameStart, true);