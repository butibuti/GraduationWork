#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ModelDrawComponent;

	class BrightnessController :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "BrightnessController";
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
			ARCHIVE_BUTI(m_isMeshDraw);
		}

		void ResetMeshDraw();

		void ResetStartColor();
		void SetBrightness(const float arg_brightness);

	private:
		float m_brightness;

		std::vector<Value_weak_ptr<MeshDrawComponent>> m_vec_vwp_meshDrawComponents;
		std::vector<Value_weak_ptr<ModelDrawComponent>> m_vec_vwp_modelDrawComponents;
		std::vector<Vector3> m_vec_startColors;

		bool m_isMeshDraw = true;
	};

}

BUTI_REGIST_GAMECOMPONENT(BrightnessController, true);