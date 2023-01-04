#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	struct CheckPoint
	{
		Vector3 pos;
		std::int32_t animFrame;
		Easing::EasingType easeType;

		template<class Archive>
		void serialize(Archive& archive)
		{
			ARCHIVE_BUTI(pos);
			ARCHIVE_BUTI(animFrame);
			ARCHIVE_BUTI(easeType);
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
			ARCHIVE_BUTI(m_vec_checkPoints);
			ARCHIVE_BUTI(m_startZoomPointNum);
		}

		bool IsStartZoomIn() { return m_passedPointCount >= m_startZoomPointNum; }
		bool IsStartZoomOut() { return m_vec_checkPoints.size() == 1; }

		void StartMove();
		void StopMove();
	private:
		void SetNewCheckPoint();

		bool m_isStart;
		Value_ptr<RelativeTimer> m_vlp_animTimer;
		std::vector<Value_ptr<CheckPoint>> m_vec_checkPoints;
		Vector3 m_startPos;
		std::int32_t m_startZoomPointNum;
		std::int32_t m_passedPointCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(Result_FriendFallPoint, true);