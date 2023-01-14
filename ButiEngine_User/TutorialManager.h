#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;
	class UI_TutorialText;

	class TutorialManager :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "TutorialManager";
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

		std::int32_t GetTutorialPhase() { return m_tutorialPhase; }

		void CheckPhaseClear(const bool arg_isFast, const bool arg_isFront);
		void SpawnBody(const Vector3& arg_spawnPosition, const float arg_moveSpeed, const float arg_rotateSpeed, const float arg_initRotationY);
	private:
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;

		std::int32_t m_tutorialPhase;

		Value_ptr<RelativeTimer> m_vlp_waitTimer;
		Value_weak_ptr<UI_TutorialText> m_vwp_tutorialText;

		bool m_isClear;
		bool m_isSceneEnd;
	};

}

BUTI_REGIST_GAMECOMPONENT(TutorialManager, true);