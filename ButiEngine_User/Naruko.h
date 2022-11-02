#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class InputManager;
	class RigidBodyComponent;

	class Naruko :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Naruko";
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
			ARCHIVE_BUTI(m_index);
		}

		void Dead();
	private:
		void Move();
		void MoveByGamePad();
		void MoveByVRTracker();

		Value_weak_ptr<InputManager> m_vwp_inputManager;
		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBodyComponent;

		std::int32_t m_index;
		Value_weak_ptr<GameObject> m_vwp_drawObject;
	};

}

BUTI_REGIST_GAMECOMPONENT(Naruko, true);