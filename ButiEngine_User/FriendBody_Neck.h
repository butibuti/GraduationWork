#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class FriendBody_Neck :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FriendBody_Neck";
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

		void SetParent(Value_weak_ptr<GameObject> arg_vwp_parent) 
		{
			m_vwp_parent = arg_vwp_parent; 
			gameObject.lock()->transform->SetBaseTransform(arg_vwp_parent.lock()->transform, true);
		}
		Value_weak_ptr<GameObject> GetParent() { return m_vwp_parent; }
	private:
		Value_weak_ptr<GameObject> m_vwp_parent;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendBody_Neck, true);