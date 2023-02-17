#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class InputManager;
	class GameSettings;
	class RigidBodyComponent;
	class PauseManager;
	class FriendManager;
	class FriendHead_PartHitArea;
	class FriendHead_Center;
	class StageManager;
	class TutorialManager;

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
			ARCHIVE_BUTI(m_fastBorder);
		}

		Value_weak_ptr<GameObject> GetEye();
		Value_weak_ptr<GameObject> GetNose();
		Value_weak_ptr<GameObject> GetMouth();

		std::int32_t GetEyeScore();
		std::int32_t GetNoseScore();
		std::int32_t GetMouthScore();

		bool IsFast();
		bool IsGood();

		bool IsExistPartStuckArea();
		void LeavePartRandom();

		void Blow();
		void Dead();

		void Disappear();
	private:
		void Control();
		void ControlByKeyboard();
		void ControlByGamePad();
		void ControlByVRTracker();
		void SpawnStarFlash();
		void OnPut(Value_weak_ptr<GameObject> arg_vwp_body);
		void CompleteFace();

		void Appear();
		void CalcVelocity();
		Vector3 GetTrackerPos();
		Matrix4x4 GetTrackerRotation();
		void CheckPut();
		bool CanPut();
		bool CanUpdate();
		std::vector<Value_weak_ptr<FriendHead_PartHitArea>> GetPartStuckAreas();

		void CreatePartHitArea();

		Value_weak_ptr<StageManager> m_vwp_stageManager;
		Value_weak_ptr<InputManager> m_vwp_inputManager;
		Value_weak_ptr<GameSettings> m_vwp_gameSettings;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<FriendManager> m_vwp_friendManager;

		std::int32_t m_trackerIndex;

		Value_weak_ptr<GameObject> m_vwp_headCenter;
		Value_weak_ptr<FriendHead_Center> m_vwp_headCenterComponent;

		//移動速度関連
		Vector3 m_prevPos;
		Vector3 m_crntPos;
		Vector3 m_velocity;

		//各パーツがくっつく範囲
		Value_weak_ptr<GameObject> m_vwp_eyesHitArea;
		Value_weak_ptr<GameObject> m_vwp_noseHitArea;
		Value_weak_ptr<GameObject> m_vwp_mouthHitArea;
		Value_weak_ptr<GameObject> m_vwp_dummyHitArea;

		Value_weak_ptr<FriendHead_PartHitArea> m_vwp_eyesHitAreaComponent;
		Value_weak_ptr<FriendHead_PartHitArea> m_vwp_noseHitAreaComponent;
		Value_weak_ptr<FriendHead_PartHitArea> m_vwp_mouthHitAreaComponent;
		Value_weak_ptr<FriendHead_PartHitArea> m_vwp_dummyHitAreaComponent;

		Value_ptr<RelativeTimer> m_vlp_appearTimer;

		bool m_isPut;

		bool m_isCompleteFace;

		bool m_isFast;
		std::int32_t m_fastBorder;
		Value_ptr<RelativeTimer> m_vlp_completeFaceCountUpTimer;

		Value_ptr<RelativeTimer> m_vlp_spawnStarFlashIntervalTimer;

		bool m_isTutorial;
		Value_weak_ptr<TutorialManager> m_vwp_tutorialManager;

		bool m_isShowGuide;

		bool m_isBlow;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendHead, true);