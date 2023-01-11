#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SceneTransition_Fade :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "SceneTransition_Fade";
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
			ARCHIVE_BUTI(m_isSceneEnd);
			ARCHIVE_BUTI(m_waitFrame);
			ARCHIVE_BUTI(m_transitionFrame);
			ARCHIVE_BUTI(m_nextSceneName);
		}

		void SetNextSceneName(const std::string& arg_nextSceneName) { m_nextSceneName = arg_nextSceneName; }
	private:
		Vector4 m_startColor;
		bool m_isSceneEnd;
		std::int32_t m_waitFrame;
		std::int32_t m_transitionFrame;
		std::string m_nextSceneName;

		Value_ptr<RelativeTimer> m_vlp_waitTimer;
		Value_ptr<RelativeTimer> m_vlp_fadeTimer;
		Value_weak_ptr<MeshDrawComponent> m_vwp_meshDraw;
	};

}

BUTI_REGIST_GAMECOMPONENT(SceneTransition_Fade, true);