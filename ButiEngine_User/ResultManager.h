#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Result_Camera;
	class Result_BackHuman;
	class Result_FriendFallPoint;

	class ResultManager :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "ResultManager";
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
			ARCHIVE_BUTI(g_successBorder);
		}

		static std::int32_t GetSuccessBorder() { return g_successBorder; }

		void StartFailedTimer();
	private:
		void CheckStartZoomIn();
		void CheckStartZoomOut();
		void StartSuccess();
		void StartFailed();

		static std::int32_t g_successBorder;

		Value_weak_ptr<Result_Camera> m_vwp_camera;
		Value_weak_ptr<Result_BackHuman> m_vwp_backHuman;
		Value_weak_ptr<Result_FriendFallPoint> m_vwp_fallPoint;

		Value_ptr<RelativeTimer> m_vlp_waitStartFallTimer;

		bool m_isStartedZoomIn;
		bool m_isStartedZoomOut;
	};

}

BUTI_REGIST_GAMECOMPONENT(ResultManager, true);