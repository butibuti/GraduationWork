#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	
	class RigidBodyComponent;

	class Effect_Box :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Effect_Box";
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
		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBodyComponent;
	};

}

BUTI_REGIST_GAMECOMPONENT(Effect_Box, true);