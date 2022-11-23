#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class InputManager;
	class GameSettings;
	class RigidBodyComponent;
	class PauseManager;
	class FriendManager;

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

		bool CanStickEye() { return m_eyeCount < m_maxEyeCount; }
		bool CanStickNose() { return m_noseCount < m_maxNoseCount; }
		bool CanStickMouth() { return m_mouthCount < m_maxMouthCount; }
		
		void AddEyeCount() { m_eyeCount++; }
		void AddNoseCount() { m_noseCount++; }
		void AddMouthCount() { m_mouthCount++; }

		void Dead();
	private:
		void Control();
		void ControlByGamePad();
		void ControlByVRTracker();
		void OnPut();

		void CalcVelocity();
		void CheckPut();
		bool CanPut();

		Value_weak_ptr<InputManager> m_vwp_inputManager;
		Value_weak_ptr<GameSettings> m_vwp_gameSettings;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<FriendManager> m_vwp_friendManager;

		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBodyComponent;

		std::int32_t m_trackerIndex;

		//移動速度関連
		Vector3 m_prevPos;
		Vector3 m_crntPos;
		Vector3 m_velocity;

		//各パーツの数
		std::int32_t m_eyeCount;
		std::int32_t m_noseCount;
		std::int32_t m_mouthCount;

		//各パーツのつけられる上限
		std::int32_t m_maxEyeCount;
		std::int32_t m_maxNoseCount;
		std::int32_t m_maxMouthCount;

		//台に置いたか確認する用
		Value_ptr<RelativeTimer> m_vlp_putTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendHead, true);