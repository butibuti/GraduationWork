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

		bool CanStickEye() { return m_vec_eyes.size() < m_maxEyeCount; }
		bool CanStickNose() { return m_vec_noses.size() < m_maxNoseCount; }
		bool CanStickMouth() { return m_vec_mouths.size() < m_maxMouthCount; }
		
		void StickEye(Value_weak_ptr<GameObject> arg_eye) { m_vec_eyes.push_back(arg_eye); }
		void StickNose(Value_weak_ptr<GameObject> arg_nose) { m_vec_noses.push_back(arg_nose); }
		void StickMouth(Value_weak_ptr<GameObject> arg_mouth) { m_vec_mouths.push_back(arg_mouth); }

		void Dead();
	private:
		void Control();
		void ControlByGamePad();
		void ControlByVRTracker();
		void OnPut();
		void SpawnNewHead();

		void CalcVelocity();
		void CheckPut();
		void CheckSpawnBody();
		bool CanPut();

		Value_weak_ptr<InputManager> m_vwp_inputManager;
		Value_weak_ptr<GameSettings> m_vwp_gameSettings;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<FriendManager> m_vwp_friendManager;

		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBodyComponent;

		Value_weak_ptr<GameObject> m_vwp_body;

		std::int32_t m_trackerIndex;

		//移動速度関連
		Vector3 m_prevPos;
		Vector3 m_crntPos;
		Vector3 m_velocity;

		//くっついているパーツ
		std::vector<Value_weak_ptr<GameObject>> m_vec_eyes;
		std::vector<Value_weak_ptr<GameObject>> m_vec_noses;
		std::vector<Value_weak_ptr<GameObject>> m_vec_mouths;

		//各パーツのつけられる上限
		std::int32_t m_maxEyeCount;
		std::int32_t m_maxNoseCount;
		std::int32_t m_maxMouthCount;

		//各パーツの基準位置
		Vector3 m_leftEyeStandardPos;
		Vector3 m_rightEyeStandardPos;
		Vector3 m_noseStandardPos;
		Vector3 m_mouthStandardPos;

		//台に置いたか確認する用
		Value_ptr<RelativeTimer> m_vlp_putTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendHead, true);