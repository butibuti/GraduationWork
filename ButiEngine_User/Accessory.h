#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Accessory :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Accessory";
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
		void OnCollisionPartHitArea(Value_weak_ptr<GameObject> arg_vwp_partHitArea);
		void Appear();
		void OnAppear();

		Value_weak_ptr<GameObject> m_vwp_drawObject;
		bool m_isAppear;
	};

}

BUTI_REGIST_GAMECOMPONENT(Accessory, true);