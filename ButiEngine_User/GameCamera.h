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
		void StartLookAtTarget(const Vector3& arg_lookTargetPos, const std::int32_t arg_lookFrame);
	private:
		void AddPositionAnimation(const Vector3& arg_targetPos, const std::int32_t arg_animFrame);
		void LookAtTarget();

		Value_weak_ptr<GameObject> m_vwp_cameraMan;

		Vector3 m_startPos;

		Value_ptr<RelativeTimer> m_vlp_waitShakeTimer;

		Vector3 m_lookTargetPos;
		Matrix4x4 m_zoomStartRotation;
		Value_ptr<RelativeTimer> m_vlp_lookTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(GameCamera, true);