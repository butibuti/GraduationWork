#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	enum class SoldierState
	{
		Sleep,
		Active,
		Home,
		Abduction,
	};

	class RigidBodyComponent;

	class Soldier :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Soldier";
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
			ARCHIVE_BUTI(m_detectionRange);
		}

		SoldierState GetState() { return m_state; }

		void Dead();
		void Sleep();
		void Abduction(Value_weak_ptr<GameObject> arg_parent);
	private:
		void OnSleep();
		void Move();
		void SetMoveDirection(Vector2& arg_ref_moveDirection);
		void SetMoveSpeed();

		Value_weak_ptr<GameObject> m_vwp_drawObject;
		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBodyComponent;

		Value_weak_ptr<GameObject> m_vwp_naruko;

		SoldierState m_state;
		float m_detectionRange;
		float m_moveSpeed;
	};

}

BUTI_REGIST_GAMECOMPONENT(Soldier, true);