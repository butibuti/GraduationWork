#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	enum class PartType
	{
		Eye,
		Nose,
		Mouth,
		Dummy,
	};

	enum class Rank
	{
		NoRank = -2,
		Bad = -1,
		Normal = 0,
		Good = 1,
	};

	struct FacePartParameter
	{
		PartType type = PartType::Eye;

		std::int32_t life = 600;

		Vector3 velocity = Vector3Const::Zero;
		float moveSpeed = 1.0f;
		float maxMoveSpeed = 1.0f;

		bool isGravity = false;
		float gravity = 0.0f;
		Vector3 gravityDirection = -Vector3Const::YAxis;

		bool isSway = true;
		float rotateSpeed = 0.0f;

		template<class Archive>
		void serialize(Archive& archive)
		{
			ARCHIVE_BUTI(type);
			ARCHIVE_BUTI(life);
			ARCHIVE_BUTI(velocity);
			ARCHIVE_BUTI(moveSpeed);
			ARCHIVE_BUTI(maxMoveSpeed);
			ARCHIVE_BUTI(isGravity);
			ARCHIVE_BUTI(gravity);
			ARCHIVE_BUTI(gravityDirection);
			ARCHIVE_BUTI(isSway);
			ARCHIVE_BUTI(rotateSpeed);
		}

		bool GUI_SetPartType();
		bool GUI_SetLife();
		bool GUI_SetMoveParam();
		bool GUI_SetGravityParam();
		bool GUI_SetRotationParam();
	};

	class StageManager;
	class PauseManager;
	class GameLevelManager;
	class PartRespawnPoint;

	class FriendFacePart :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FriendFacePart";
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
			ARCHIVE_BUTI(m_param);
			ARCHIVE_BUTI(g_goodAngleBorder);
		}

		void Dead();

		void SetRandomParam_Straight();

		void LeaveHead();

		void RemoveStickAnimation();

		std::int32_t GetCalcScore();
		std::int32_t GetCalcAngleScore();
		std::int32_t GetCalcPosScore();

		Rank GetCalcPartRank();
		Rank GetPartRank() { return m_rank; }

		bool IsGood() { return IsGoodAngle() && IsGoodPos(); }
		bool IsGoodAngle();
		bool IsGoodPos();

		static void ResetPartCount()
		{
			g_eyeCount = 0;
			g_noseCount = 0;
			g_mouthCount = 0;
			g_dummyCount = 0;
		}

		static std::int32_t GetEyeCount() { return g_eyeCount; }
		static std::int32_t GetNoseCount() { return g_noseCount; }
		static std::int32_t GetMouthCount() { return g_mouthCount; }
		static std::int32_t GetNormalPartCount() { return g_eyeCount + g_noseCount + g_mouthCount; }
		static std::int32_t GetDummyPartCount() { return g_dummyCount; }

		static std::int32_t GetGoodScoreBorder() { return g_goodScoreBorder; }
		static std::int32_t GetNormalScoreBorder() { return g_normalScoreBorder; }

		void SetRespawnPoint(Value_weak_ptr<PartRespawnPoint> arg_vwp_respawnPoint) { m_vwp_respawnPoint = arg_vwp_respawnPoint; }
		void SetParam(const FacePartParameter& arg_param) { m_param = arg_param; }
	private:
		void Move();
		void Rotate();

		void StickHead();

		void SpawnStickEffect();
		void SpawnBeamEffect();
		void SpawnDummyPartHitEffect();

		void OnCollisionPartHitArea(Value_weak_ptr<GameObject> arg_vwp_partHitArea);

		bool CanUpdate();
		Vector3 GetStickPos();
		void Blow();
		void CheckRank();
		void ChangeModel();

		void Appear();
		void Disappear();
		void OnAppear();
		void OnDisappear();
		void AddScaleAnimation(const Vector3& arg_targetScale, Easing::EasingType arg_easeType);

		void AddPartCount();
		void RemovePartCount();

		bool IsBetterRank();
		void CreateEvaluationObject();


		Value_weak_ptr<StageManager> m_vwp_stageManager;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;

		Value_ptr<RelativeTimer> m_vlp_deadTimer, m_vlp_lifeTimer;

		Value_weak_ptr<PartRespawnPoint> m_vwp_respawnPoint;

		bool m_isAppear;
		bool m_isDisappear;

		bool m_isMove;
		FacePartParameter m_param;

		Value_weak_ptr<GameObject> m_vwp_head;
		Value_weak_ptr<GameObject> m_vwp_partHitArea;

		Value_ptr<RelativeTimer> m_vlp_leaveIntervalTimer;

		float m_startZ;

		Rank m_rank;
		static Vector3 g_goodAngleBorder;

		bool m_isTutorial;

		static std::int32_t g_eyeCount;
		static std::int32_t g_noseCount;
		static std::int32_t g_mouthCount;
		static std::int32_t g_dummyCount;

		static std::int32_t g_normalScoreBorder;
		static std::int32_t g_goodScoreBorder;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendFacePart, true);