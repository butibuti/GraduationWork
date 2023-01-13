#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class StageManager;
	class PauseManager;

	class Heart;
	class GameTimer :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "GameTimer";
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
			ARCHIVE_BUTI(m_countSecond);
		}

		void SetCountSecond(std::int32_t arg_countSecond);
		std::int32_t GetCountSecond() { return m_countSecond; }
		std::int32_t GetRemainSecond();

		void StartTimer();
		void StopTimer();
		void ResetTimer();
	private:
		bool CanUpdate();

		Value_weak_ptr<StageManager> m_vwp_stageManager;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<Heart> m_vwp_heart;

		Value_ptr<RelativeTimer> m_vlp_timer;
		std::int32_t m_countSecond;
	};

}

BUTI_REGIST_GAMECOMPONENT(GameTimer, true);