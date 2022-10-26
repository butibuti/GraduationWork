#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RigidBodyComponent;

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
		}

		void Dead();
	private:
		Value_weak_ptr<GameObject> m_vwp_home;
		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBodyComponent;
	};

}

BUTI_REGIST_GAMECOMPONENT(Enemy_Test, true);