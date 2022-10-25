#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class InputManager :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "InputManager";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		//ゲーム全体
		bool IsPushRightKey();
		bool IsPushLeftKey();
		bool IsPushUpKey();
		bool IsPushDownKey();
		bool IsTriggerRightKey();
		bool IsTriggerLeftKey();
		bool IsTriggerUpKey();
		bool IsTriggerDownKey();
		bool IsTriggerDecideKey();
		bool IsTriggerCancelKey();
		bool IsPushAnyGameKey();
		Vector2 GetMousePosition();

		Vector2 GetRightStick();
		Vector2 GetLeftStick();
		float GetRightTrigger();
		float GetLeftTrigger();
		bool IsNotPushKey() { return m_isNotPushKey; }

		void SetRightVibrationPower(const float arg_power);
		void SetLeftVibrationPower(const float arg_power);

		//ゲームプレイシーンのみ

	private:
		bool m_isNotPushKey;
		Vector2 m_previousRightStick;
		Vector2 m_currentRightStick;
		Vector2 m_previousLeftStick;
		Vector2 m_currentLeftStick;
	};

}

BUTI_REGIST_GAMECOMPONENT(InputManager, true);