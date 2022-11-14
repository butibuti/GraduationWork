#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	enum class MovePattern
	{
		Straight,
	};

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

		void OnCollisionFriendHead(Value_weak_ptr<GameObject> arg_vwp_gameObject);

		MovePattern m_movePattern;

		bool m_canMove;
		Vector3 m_moveDirection;
		float m_moveSpeed;
		float m_minMoveSpeed;
		float m_maxMoveSpeed;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendFacePart, true);