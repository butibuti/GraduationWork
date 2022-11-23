#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	struct GameSettingsData
	{
		std::int32_t trackerIndex = 0;

		Matrix4x4 trackerOrigin;
		Vector3 headMoveLimit = Vector3Const::Zero;
		Vector3 moveAreaFrontRightTop = Vector3Const::Zero;
		Vector3 moveAreaBackLeftBottom = Vector3Const::Zero;
		Vector3 tablePos = Vector3Const::Zero;
		Vector3 bodyPos = Vector3Const::Zero;
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
		Vector3 GetCorrection();
		Vector3 GetTablePos() { return m_data.tablePos; }
		Vector3 GetBodyPos() { return m_data.bodyPos; }

	private:
		void SetOrigin();
		void SetMoveAreaFrontRightTop();
		void SetMoveAreaBackLeftBottom();
		void SetTablePos();

		GameSettingsData m_data;
	};

}

BUTI_REGIST_GAMECOMPONENT(GameSettings, true);