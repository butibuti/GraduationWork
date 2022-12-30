#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	struct FriendData;

	class Result_CompleteFriend :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Result_CompleteFriend";
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

		void CreateParts(Value_weak_ptr<FriendData> arg_vwp_friendData);
	private:
		void Fall();
		void CheckFall();

		Value_weak_ptr<GameObject> m_vwp_body;
		Value_ptr<ButiRendering::IAnimationController> m_vlp_animationController;

		Value_weak_ptr<GameObject> m_vwp_fallPoint;

		bool m_isFall;
		Vector3 m_velocity;
	};

}

BUTI_REGIST_GAMECOMPONENT(Result_CompleteFriend, true);