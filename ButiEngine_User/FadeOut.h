#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class FadeOut :public GameComponent
	{
	public:
		FadeOut(){}
		FadeOut(const std::int32_t arg_fadeFrame) : m_fadeFrame(arg_fadeFrame){}
		std::string GetGameComponentName()const override {
			return "FadeOut";
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
		Value_weak_ptr<MeshDrawComponent> m_vwp_meshDraw;

		float m_startAlpha;

		Value_ptr<RelativeTimer> m_vlp_timer;
		std::int32_t m_fadeFrame;
	};

}

BUTI_REGIST_GAMECOMPONENT(FadeOut, true);