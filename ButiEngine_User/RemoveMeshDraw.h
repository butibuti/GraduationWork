#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RemoveMeshDraw :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "RemoveMeshDraw";
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
	};

}

BUTI_REGIST_GAMECOMPONENT(RemoveMeshDraw, true);