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
			ARCHIVE_BUTI(m_minMoveSpeed);
			ARCHIVE_BUTI(m_maxMoveSpeed);
			ARCHIVE_BUTI(m_type);
		}

		void SetMovePattern(const MovePattern arg_movePattern) { m_movePattern = arg_movePattern; }

		void Dead();
	private:
		void Move();
		void MoveStay();
		void MoveStraight();
		void MoveThrow();
		void SetMoveDirection();
		void StickToHead();
		void StickEffect();

		void StartChase();
		void Chase();

		void ChangeGroupMask();

		void OnCollisionPartHitArea(Value_weak_ptr<GameObject> arg_vwp_partHitArea);

		bool CanUpdate();
		Vector3 GetChaseTargetPos();

		Value_weak_ptr<StageManager> m_vwp_stageManager;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;

		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBodyComponent;

		Value_ptr<RelativeTimer> m_vlp_deadTimer;
		Value_ptr<RelativeTimer> m_vlp_lifeTimer;

		Value_ptr<RelativeTimer> m_vlp_changeGroupMaskTimer;

		PartType m_type = PartType::Eye;
		
		MovePattern m_movePattern = MovePattern::Stay;

		bool m_isCollisionHead;
		Vector3 m_moveDirection;
		float m_moveSpeed;
		float m_minMoveSpeed;
		float m_maxMoveSpeed;


		FacePartState m_state;

		Value_weak_ptr<GameObject> m_vwp_partHitArea;
		Vector3 m_chaseStartPos;
		Value_weak_ptr<GameObject> m_vwp_chaseTarget;
		Value_ptr<RelativeTimer> m_vlp_lockOnTimer;
		Value_ptr<RelativeTimer> m_vlp_chaseTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendFacePart, true);