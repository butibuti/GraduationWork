#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Result_Camera :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Result_Camera";
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

		void ZoomIn();
		void ZoomOut();
	private:
		void AddPositionAnimation(const Vector3& arg_targetPos, const std::int32_t arg_animFrame, const Easing::EasingType arg_easeType);

		Vector3 m_startPos;
		Vector3 m_zoomTargetPos;
	};

}

BUTI_REGIST_GAMECOMPONENT(Result_Camera, true);