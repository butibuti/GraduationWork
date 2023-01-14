#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	struct FriendData;
	class PauseManager;

	class BonusFriend :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "BonusFriend";
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
		
		void SetFrontBorder(const float arg_border) { m_frontBorder = arg_border; }

		void CreateParts(Value_weak_ptr<FriendData> arg_vwp_friendData);
		void Appear(const std::int32_t arg_friendNum);
		void StartMoveBack();
	private:
		void StartDance();
		void MoveBack();
		void Rotate();
		void StopRotate();
		float GetLookForwardHeadAngle();

		Vector3 GetFrontXZ(const Vector3& arg_front);

		bool IsFrontHead();

		Value_weak_ptr<PauseManager> m_vwp_pauseManager;

		Value_weak_ptr<GameObject> m_vwp_head;
		Value_weak_ptr<GameObject> m_vwp_body;
		Value_ptr<ButiRendering::IAnimationController> m_vlp_animationController;

		float m_frontBorder;
		bool m_isMoveBack;
		bool m_isRotate;
		bool m_isStopRotate;
		bool m_isDance;

		Vector3 m_moveBackStartPos;
		Vector3 m_moveBackTargetPos;
		Value_ptr<RelativeTimer> m_vlp_moveBackTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(BonusFriend, true);