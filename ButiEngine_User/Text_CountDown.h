#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Text_CountDown :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Text_CountDown";
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

		void SetNumber(const std::int32_t arg_num);

	private:
		std::int32_t m_number;

		Value_ptr<RelativeTimer> m_vlp_lifeTimer;
		Value_ptr<RelativeTimer> m_vlp_deadTimer;

		Value_weak_ptr<MeshDrawComponent> m_vwp_meshDraw;
		Vector4 m_color;
		float m_startAlpha;
	};

}

BUTI_REGIST_GAMECOMPONENT(Text_CountDown, true);