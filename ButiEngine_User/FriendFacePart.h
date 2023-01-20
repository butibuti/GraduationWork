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
		Chase,
		Stop,
	};

	enum class MovePattern
	{
		Stay,
		Straight,
		Throw,
	};

	class StageManager;
	class PauseManager;
	class RigidBodyComponent;
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
			ARCHIVE_BUTI(m_type);
			ARCHIVE_BUTI(m_vec_minStraightMoveSpeeds);
			ARCHIVE_BUTI(m_vec_maxStraightMoveSpeeds);
			ARCHIVE_BUTI(m_vec_minThrowMoveSpeeds);
			ARCHIVE_BUTI(m_vec_maxThrowMoveSpeeds);
			ARCHIVE_BUTI(m_vec_stayProbabilities);
			ARCHIVE_BUTI(m_vec_straightProbabilities);
			ARCHIVE_BUTI(m_vec_throwProbabilities);
		}

		void SetMovePattern(const std::int32_t arg_gameLevel);
		MovePattern GetMovePattern() { return m_movePattern; }

		void Dead();

		static void ResetPartCount()
		{
			g_eyeCount = 0;
			g_noseCount = 0;
			g_mouthCount = 0;
		}

		static std::int32_t GetEyeCount() { return g_eyeCount; }
		static std::int32_t GetNoseCount() { return g_noseCount; }
		static std::int32_t GetMouthCount() { return g_mouthCount; }
	private:
		void Move();
		void MoveStay();
		void MoveStraight();
		void MoveThrow();
		void InitThrow();

		void SetMoveDirection();
		void SetMoveSpeed();

		void StickToHead();
		void StickEffect();

		void StartChase();
		void Chase();

		void OnCollisionPartHitArea(Value_weak_ptr<GameObject> arg_vwp_partHitArea);

		bool CanUpdate();
		Vector3 GetChaseTargetPos();

		void ResizeLevelParameter();

		Value_weak_ptr<StageManager> m_vwp_stageManager;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;

		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBodyComponent;

		Value_ptr<RelativeTimer> m_vlp_deadTimer;
		Value_ptr<RelativeTimer> m_vlp_lifeTimer;

		std::vector<float> m_vec_minStraightMoveSpeeds;
		std::vector<float> m_vec_maxStraightMoveSpeeds;
		std::vector<float> m_vec_minThrowMoveSpeeds;
		std::vector<float> m_vec_maxThrowMoveSpeeds;
		std::vector<float> m_vec_stayProbabilities;
		std::vector<float> m_vec_straightProbabilities;
		std::vector<float> m_vec_throwProbabilities;

		PartType m_type = PartType::Eye;
		
		MovePattern m_movePattern = MovePattern::Stay;

		bool m_isCollisionHead;
		Vector3 m_moveDirection;
		float m_moveSpeed;

		FacePartState m_state;

		Value_weak_ptr<GameObject> m_vwp_partHitArea;
		Vector3 m_chaseStartPos;
		Value_weak_ptr<GameObject> m_vwp_chaseTarget;
		Value_ptr<RelativeTimer> m_vlp_chaseTimer;

		bool m_isTutorial;

		static std::int32_t g_eyeCount;
		static std::int32_t g_noseCount;
		static std::int32_t g_mouthCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendFacePart, true);