#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Debug_Camera :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Debug_Camera";
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
		bool m_isDebug;
	};

}

BUTI_REGIST_GAMECOMPONENT(Debug_Camera, true);