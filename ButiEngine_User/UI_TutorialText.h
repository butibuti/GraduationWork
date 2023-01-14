#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class UI_TutorialText :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "UI_TutorialText";
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

		void Clear();

	private:
		Value_ptr<RelativeTimer> m_vlp_waitDisappearTimer;
		Value_ptr<RelativeTimer> m_vlp_disappearTimer;
		Value_weak_ptr<MeshDrawComponent> m_vwp_meshDraw;

		Vector3 m_startScale;
	};

}

BUTI_REGIST_GAMECOMPONENT(UI_TutorialText, true);