#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ShakeComponent :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "ShakeComponent";
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

		void SetShakeFrame(const std::int32_t arg_ShakeComponentFrame) { m_vlp_shakeTimer->ChangeCountFrame(arg_ShakeComponentFrame); }
		void SetStartAmplitude(const Vector3& arg_amplitude) { m_startAmplitude = arg_amplitude; }
		void SetShakeSpeed(const float arg_speed) { m_shakeSpeed = arg_speed; }

		void Dead();

	private:
		void ResetRotation();

		Value_ptr<RelativeTimer> m_vlp_shakeTimer;

		Matrix4x4 m_startRotation;
		Vector3 m_startAmplitude;
		float m_theta;
		float m_shakeSpeed;
	};

}

BUTI_REGIST_GAMECOMPONENT(ShakeComponent, true);