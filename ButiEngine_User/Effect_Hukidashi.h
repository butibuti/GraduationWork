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
		void AddScaleAnimation(const Vector3& arg_targetScale, const std::int32_t arg_animFrame, const Easing::EasingType arg_easeType);

		Value_ptr<RelativeTimer> m_vlp_appearTimer;
		Value_ptr<RelativeTimer> m_vlp_lifeTimer;

		float m_scaleMagnification;
	};

}

BUTI_REGIST_GAMECOMPONENT(Effect_Hukidashi, true);