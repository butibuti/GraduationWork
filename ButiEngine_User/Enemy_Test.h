#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiBullet{
	class Joint_spring;
}

namespace ButiEngine {

	enum class EnemyState
	{
		Chase,
		Return,
	};

	class RigidBodyComponent;
	class SoldierManager;
	class Soldier;

	class Enemy_Test :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Enemy_Test";
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
		}

		void Dead();
	private:
		void Move();
		void SetTargetSoldier();
		void OnReturn();
		void CreateJoint();
		void DestroyJoint();

		Value_weak_ptr<SoldierManager> m_vwp_soldierManager;
		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBodyComponent;

		EnemyState m_state;

		Value_weak_ptr<GameObject> m_vwp_targetSoldier;
		Value_weak_ptr<GameObject> m_vwp_abductionSoldier;
		Value_weak_ptr<ButiBullet::Joint_spring> m_vwp_joint;
		Vector3 m_spawnPos;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy_Test, true);