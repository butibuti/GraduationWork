#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class FriendHead_Center :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FriendHead_Center";
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

		Value_weak_ptr<GameObject> GetCollisionFriendBody() { return m_vwp_collisionFriendBody; }
	private:
		Value_weak_ptr<GameObject> m_vwp_collisionFriendBody;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendHead_Center, true);