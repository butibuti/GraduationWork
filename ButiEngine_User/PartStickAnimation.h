#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

	class PartStickAnimation :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "PartStickAnimation";
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

		Value_ptr<RelativeTimer> m_vlp_aniimationTimer;

		Vector3 m_standardScale;
		float m_startAmplitude;
		float m_theta;
		float m_animationSpeed;
	};

}

BUTI_REGIST_GAMECOMPONENT(PartStickAnimation, true);