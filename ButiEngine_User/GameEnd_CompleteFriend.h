#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	struct FriendData;

	class GameEnd_CompleteFriend :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "GameEnd_CompleteFriend";
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
		Value_ptr<ButiRendering::IAnimationController> m_vlp_animationController;
	};

}

BUTI_REGIST_GAMECOMPONENT(GameEnd_CompleteFriend, true);