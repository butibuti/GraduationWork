#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	struct CheckPoint
	{
		float pos;
		float targetSpeed;
		std::int32_t changeSpeedFrame;
		Easing::EasingType easeType;
		bool hasLimit;
		std::int32_t limitFrame;

		template<class Archive>
		void serialize(Archive& archive)
		{
			ARCHIVE_BUTI(pos);
			ARCHIVE_BUTI(targetSpeed);
			ARCHIVE_BUTI(changeSpeedFrame);
			ARCHIVE_BUTI(easeType);
			ARCHIVE_BUTI(hasLimit);
			ARCHIVE_BUTI(limitFrame);
		}
	};

	class Result_FriendFallPoint :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Result_FriendFallPoint";
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
			ARCHIVE_BUTI(m_initSpeed);
			ARCHIVE_BUTI(m_vec_checkPoints);
			ARCHIVE_BUTI(m_startZoomPointNum);
		}

		bool IsStartZoomIn() { return m_passedPointCount >= m_startZoomPointNum; }
		bool IsStartZoomOut() { return m_passedPointCount >= m_vec_checkPoints.size() - 1; }

		void StartMove();
		void StopMove();
	private:
		void SetNextCheckPoint();

		bool m_isStart;
		float m_moveSpeed;
		float m_initSpeed;
		//CheckPoint通過時のスピード
		float m_startSpeed;
		Value_ptr<RelativeTimer> m_vlp_changeSpeedTimer;
		std::vector<Value_ptr<CheckPoint>> m_vec_checkPoints;
		Value_ptr<CheckPoint> m_vlp_passedCheckPoint;
		Value_ptr<CheckPoint> m_vlp_nextCheckPoint;
		std::int32_t m_startZoomPointNum;
		std::int32_t m_passedPointCount;
		Value_ptr<RelativeTimer> m_vlp_limitTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(Result_FriendFallPoint, true);