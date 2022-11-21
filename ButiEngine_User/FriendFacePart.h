#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	enum class FacePartState
	{
		Move,
		Chase,
		Stop,
	};

	enum class MovePattern
	{
		Straight,
	};

	class StageManager;
	class PauseManager;

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
		}

	private:
		void Move();
		void MoveStraight();
		void SetMoveDirection();

		void OnChase();

		void OnCollisionFriendHead(Value_weak_ptr<GameObject> arg_vwp_gameObject);

		Value_weak_ptr<StageManager> m_vwp_stageManager;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		
		MovePattern m_movePattern;

		bool m_canMove;
		Vector3 m_moveDirection;
		float m_moveSpeed;
		float m_minMoveSpeed;
		float m_maxMoveSpeed;


		FacePartState m_state;
		Value_weak_ptr<GameObject> m_vwp_head;
		Value_weak_ptr<GameObject> m_vwp_chaseTarget;
		Value_ptr<RelativeTimer> m_vlp_lockOnTimer;
		Value_ptr<RelativeTimer> m_vlp_chaseTimer;

		bool m_isRayCast;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendFacePart, true);