#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "PauseManager";
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

		void SetIsPause(const bool arg_isPause);
		bool IsPause() { return m_isPause; }

		void SetIsAllPause(const bool arg_isAllPause)
		{
			if (arg_isAllPause)
			{
				GameDevice::SetWorldSpeed(0.0f);
			}
			else
			{
				GameDevice::SetWorldSpeed(1.0f);
			}
		}

	private:
		bool m_isPause;
	};

}

BUTI_REGIST_GAMECOMPONENT(PauseManager, true);