#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

	class SwayAnimation :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "SwayAnimation";
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
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;

		Value_ptr<RelativeTimer> m_vlp_animTimer;

		float m_startAngle;
		float m_targetAngle;
	};

}

BUTI_REGIST_GAMECOMPONENT(SwayAnimation, true);