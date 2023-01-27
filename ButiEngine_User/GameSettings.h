#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	struct GameSettingsData
	{
		std::int32_t trackerIndex;

		Matrix4x4 trackerOrigin;
		Vector3 headMoveLimit;
		Vector3 moveAreaFrontRightTop;
		Vector3 moveAreaBackLeftBottom;
		Vector3 tablePos;
		Vector3 bodyPos;
		template<class Archive>
		void serialize(Archive& archive)
		{
			ARCHIVE_BUTI(trackerOrigin);
			ARCHIVE_BUTI(trackerIndex);
			ARCHIVE_BUTI(headMoveLimit);
			ARCHIVE_BUTI(moveAreaFrontRightTop);
			ARCHIVE_BUTI(moveAreaBackLeftBottom);
			ARCHIVE_BUTI(tablePos);
			ARCHIVE_BUTI(bodyPos);
		}
	};

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
		}

		Vector3 GetHeadMoveLimit() { return m_data.headMoveLimit; }
		float GetCorrection();
		Vector3 GetTablePos() { return m_data.tablePos; }
		Vector3 GetBodyPos() { return m_data.bodyPos; }

	private:
		void SetOrigin();
		void SetMoveAreaFront();
		void SetMoveAreaRightTop();
		void SetMoveAreaBack();
		void SetMoveAreaLeftBottom();
		void SetTablePos();

		GameSettingsData m_data;

		float m_headMoveLimit;

		bool m_isDebugMode;
	};

}

BUTI_REGIST_GAMECOMPONENT(GameSettings, true);