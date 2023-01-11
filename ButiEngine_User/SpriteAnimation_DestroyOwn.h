#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class SpriteAnimationComponent;

	class SpriteAnimation_DestroyOwn : public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "SpriteAnimation_DestroyOwn";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			ARCHIVE_BUTI(isActive);
			ARCHIVE_BUTI(m_animIntervalFrame);
			ARCHIVE_BUTI(m_maxLoopCount);
		}

	private:
		void Animation();

		Value_weak_ptr<SpriteAnimationComponent> m_vwp_spriteAnimation;
		Value_ptr<RelativeTimer> m_vlp_animationIntervalTimer;

		std::int32_t m_animIntervalFrame;
		std::int32_t m_maxLoopCount;
		std::int32_t m_loopCount;
	};
}

BUTI_REGIST_GAMECOMPONENT(SpriteAnimation_DestroyOwn, true);