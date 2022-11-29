#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class InputManager;
	class GameSettings;
	class RigidBodyComponent;
	class PauseManager;
	class FriendManager;
	class FriendHead_PartHitArea;

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

		bool IsHighSpeed() { return m_velocity.z >= 0.075f; }

		std::int32_t GetEyeScore();
		std::int32_t GetNoseScore();
		std::int32_t GetMouthScore();
	private:
		void Control();
		void ControlByGamePad();
		void ControlByVRTracker();
		void OnPut();

		void CalcVelocity();
		void CheckPut();
		bool CanPut();
		bool CanUpdate();

		Value_weak_ptr<InputManager> m_vwp_inputManager;
		Value_weak_ptr<GameSettings> m_vwp_gameSettings;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<FriendManager> m_vwp_friendManager;

		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBodyComponent;

		std::int32_t m_trackerIndex;

		Value_weak_ptr<GameObject> m_vwp_headCenter;

		//移動速度関連
		Vector3 m_prevPos;
		Vector3 m_crntPos;
		Vector3 m_velocity;

		//各パーツがくっつく範囲
		Value_weak_ptr<GameObject> m_vwp_leftEyeHitArea;
		Value_weak_ptr<GameObject> m_vwp_rightEyeHitArea;
		Value_weak_ptr<GameObject> m_vwp_noseHitArea;
		Value_weak_ptr<GameObject> m_vwp_mouthHitArea;

		Value_weak_ptr<FriendHead_PartHitArea> m_vwp_leftEyeHitAreaComponent;
		Value_weak_ptr<FriendHead_PartHitArea> m_vwp_rightEyeHitAreaComponent;
		Value_weak_ptr<FriendHead_PartHitArea> m_vwp_noseHitAreaComponent;
		Value_weak_ptr<FriendHead_PartHitArea> m_vwp_mouthHitAreaComponent;

		//台に置いたか確認する用
		Value_ptr<RelativeTimer> m_vlp_putTimer;

		bool m_isPut;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendHead, true);