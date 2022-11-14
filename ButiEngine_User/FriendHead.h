#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class InputManager;
	class GameSettings;

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

		Vector3 GetVelocity() { return m_velocity; }

		void Dead();
	private:
		void Control();
		void ControlByGamePad();
		void ControlByVRTracker();

		void CalcVelocity();

		Value_weak_ptr<InputManager> m_vwp_inputManager;
		Value_weak_ptr<GameSettings> m_vwp_gameSettings;

		std::int32_t m_trackerIndex;

		Vector3 m_prevPos;
		Vector3 m_crntPos;
		Vector3 m_velocity;

	};

}

BUTI_REGIST_GAMECOMPONENT(FriendHead, true);