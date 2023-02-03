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

	enum class FacePartState
	{
		Move,
		Stick,
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
	};

	class StageManager;
	class PauseManager;
	class GameLevelManager;

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
		}

		void Dead();

		void SetRandomParam_Straight();

		void LeaveHead();

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
	private:
		void Move();

		void StickHead();

		void SpawnStickEffect();
		void SpawnDummyPartHitEffect();

		void OnCollisionPartHitArea(Value_weak_ptr<GameObject> arg_vwp_partHitArea);

		bool CanUpdate();
		Vector3 GetStickPos();
		void Blow();

		void AddPartCount();
		void RemovePartCount();

		void GUI_SetPartParam();
		void GUI_SetPartType();
		void GUI_SetLife();
		void GUI_SetMoveParam();
		void GUI_SetGravityParam();
		void GUI_SetRotationParam();

		Value_weak_ptr<StageManager> m_vwp_stageManager;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;

		Value_ptr<RelativeTimer> m_vlp_deadTimer;
		Value_ptr<RelativeTimer> m_vlp_lifeTimer;

		FacePartState m_state;
		FacePartParameter m_param;

		Value_weak_ptr<GameObject> m_vwp_head;
		Value_weak_ptr<GameObject> m_vwp_partHitArea;

		float m_startZ;
		bool m_isHitHead;

		bool m_isTutorial;

		static std::int32_t g_eyeCount;
		static std::int32_t g_noseCount;
		static std::int32_t g_mouthCount;
		static std::int32_t g_dummyCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendFacePart, true);