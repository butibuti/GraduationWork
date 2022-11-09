#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class GameSettings :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "GameSettings";
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
			ARCHIVE_BUTI(m_monitorRightTopPos);
			ARCHIVE_BUTI(m_monitorLeftBottomPos);
			ARCHIVE_BUTI(m_tablePos);
		}

	private:
		Vector3 m_monitorRightTopPos;
		Vector3 m_monitorLeftBottomPos;
		Vector3 m_tablePos;
	};

}

BUTI_REGIST_GAMECOMPONENT(GameSettings, true);