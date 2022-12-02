#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class GameCamera :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "GameCamera";
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

		void NormalZoom(const std::int32_t arg_zoomInFrame);
		void SpecialZoom(const std::int32_t arg_zoomInFrame);
		void ZoomOut(const std::int32_t arg_zoomOutFrame);
		void StartShake(const std::int32_t arg_shakeFrame);
		void StopShake();
	private:
		void AddPositionAnimation(const Vector3& arg_targetPos, const std::int32_t arg_animFrame);

		Value_weak_ptr<LookAtComponent> m_vwp_lookAt;

		Vector3 m_startPos;

		Value_ptr<RelativeTimer> m_vlp_waitShakeTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(GameCamera, true);