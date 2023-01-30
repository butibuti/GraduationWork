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

		bool isRandom = true;

		Vector3 velocity;
		float minMoveSpeed = 0.05f;
		float maxMoveSpeed = 0.075f;

		float gravity = 0.0f;
		Vector3 gravityDirection = -Vector3Const::YAxis;
		float gravityAcceleration = 0.0f;

		template<class Archive>
		void serialize(Archive& archive)
		{
			ARCHIVE_BUTI(type);
			ARCHIVE_BUTI(isRandom);
			ARCHIVE_BUTI(velocity);
			ARCHIVE_BUTI(minMoveSpeed);
			ARCHIVE_BUTI(maxMoveSpeed);
			ARCHIVE_BUTI(gravity);
			ARCHIVE_BUTI(gravityDirection);
			ARCHIVE_BUTI(gravityAcceleration);
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

		void SetParam_Stay();

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

		void StickToHead();
		void SpawnStickEffect();

		void SetRandomVelocity();

		void OnCollisionPartHitArea(Value_weak_ptr<GameObject> arg_vwp_partHitArea);

		bool CanUpdate();
		Vector3 GetStickPos();

		void AddPartCount();
		void RemovePartCount();

		void GUI_SetPartParam();
		void GUI_SetPartType();
		void GUI_SetMoveSpeed();

		Value_weak_ptr<StageManager> m_vwp_stageManager;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;

		Value_ptr<RelativeTimer> m_vlp_deadTimer;
		Value_ptr<RelativeTimer> m_vlp_lifeTimer;

		FacePartState m_state;
		FacePartParameter m_param;

		Value_weak_ptr<GameObject> m_vwp_head;
		Value_weak_ptr<GameObject> m_vwp_partHitArea;

		bool m_isTutorial;

		static std::int32_t g_eyeCount;
		static std::int32_t g_noseCount;
		static std::int32_t g_mouthCount;
		static std::int32_t g_dummyCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendFacePart, true);