#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Result_FallFriend :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Result_FallFriend";
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
		void Fall();
		void CheckFall();

		Value_weak_ptr<GameObject> m_vwp_fallPoint;

		bool m_isFallStarted;
		bool m_isFall;
		Vector3 m_velocity;

		static float g_groundedSoundVolume;
		float m_addGroundedSoundVolume;
	};

}

BUTI_REGIST_GAMECOMPONENT(Result_FallFriend, true);