#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ModelDrawComponent;

	class FlashAnimation :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FlashAnimation";
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
			ARCHIVE_BUTI(m_startColor);
			ARCHIVE_BUTI(m_targetColor);
			ARCHIVE_BUTI(m_intervalFrame);
		}

		void SetStartColor(const Vector3& arg_startColor);
		void SetTargetColor(const Vector3& arg_targetColor) { m_targetColor = arg_targetColor; }
		void SetIntervalFrame(const std::int32_t arg_frame) { m_intervalFrame = arg_frame; }
	private:
		void ChangeMeshDrawColor();
		void ChangeModelDrawColor();

		Vector3 m_startColor;
		Vector3 m_targetColor;

		std::int32_t m_intervalFrame;

		Value_weak_ptr<MeshDrawComponent> m_vwp_meshDraw;
		Value_weak_ptr<ModelDrawComponent> m_vwp_modelDraw;

		std::int32_t m_loopCount;
		std::int32_t m_changeColorCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(FlashAnimation, true);