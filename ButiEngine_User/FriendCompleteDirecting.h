#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;
	class GameCamera;

	class FriendCompleteDirecting :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FriendCompleteDirecting";
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
		Value_weak_ptr<GameObject> m_vwp_cameraMan;
		Value_weak_ptr<GameCamera> m_vwp_gameCamera;

		Value_ptr<RelativeTimer> m_vlp_directingTimer;
		Value_ptr<RelativeTimer> m_vlp_waitZoomTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendCompleteDirecting, true);