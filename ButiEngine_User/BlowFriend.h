#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class BlowFriend :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "BlowFriend";
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

	private:
		void SetBlowParam();
		void AddRollAnimation(float arg_angle);

		Vector3 m_velocity;
		float m_gravity;
	};

}

BUTI_REGIST_GAMECOMPONENT(BlowFriend, true);