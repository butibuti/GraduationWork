#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Result_BackHuman :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Result_BackHuman";
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

		void AdvanceFacePartAnimation();
		void StartTurnAnimation();
		void StartTurnSuccessAnimation();
		void StartTurnFailedAnimation();
		void ResetPartAnimation();
	private:
		Value_ptr<ButiRendering::IAnimationController> m_vlp_bodyAnimationController;
		Value_ptr<ButiRendering::IAnimationController> m_vlp_mouthAnimationController;

		bool m_isStartTurnAnimation;

		float m_mouthPoseFrame;
		float m_bodyAnimSpeed;
		float m_mouthAnimSpeed;
		float m_eyeAnimSpeed;

		float m_startEyeScale;
		float m_targetEyeScale;

		Value_ptr<Transform> m_vlp_rightEyeTransform;
		Value_ptr<Transform> m_vlp_leftEyeTransform;
		Value_ptr<RelativeTimer> m_vlp_eyeAnimTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(Result_BackHuman, true);