#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class InputManager;

	class FriendHead :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FriendHead";
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
			ARCHIVE_BUTI(m_trackerIndex);
		}

		void Dead();
	private:
		void Control();
		void ControlByGamePad();
		void ControlByVRTracker();

		Value_weak_ptr<InputManager> m_vwp_inputManager;

		std::int32_t m_trackerIndex;

	};

}

BUTI_REGIST_GAMECOMPONENT(FriendHead, true);