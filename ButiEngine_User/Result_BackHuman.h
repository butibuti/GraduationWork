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
		Value_ptr<ButiRendering::IAnimationController> m_vlp_rightEyeAnimationController;
		Value_ptr<ButiRendering::IAnimationController> m_vlp_leftEyeAnimationController;
		Value_ptr<ButiRendering::IAnimationController> m_vlp_mouthAnimationController;

		float m_partAnimSpeed;
	};

}

BUTI_REGIST_GAMECOMPONENT(Result_BackHuman, true);