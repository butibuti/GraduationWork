#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class GameSettings;
	class PauseManager;
	class GameLevelManager;
	class FriendBodySpawner;
	struct FriendData;

	class FriendBody :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FriendBody";
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
			ARCHIVE_BUTI(m_frontBorder);
			ARCHIVE_BUTI(m_vec_rotateSpeeds);
			ARCHIVE_BUTI(m_vec_moveHorizontalFrame);
		}

		Value_weak_ptr<GameObject> GetNeck() { return m_vwp_neck; }

		void SetHead(Value_weak_ptr<GameObject> arg_vwp_head);
		
		bool IsFront();
		bool IsFast();

		Value_weak_ptr<FriendBodySpawner> m_vwp_friendBodySpawner;
		const float OUT_AREA_DISTANCE = 6;

		void SetParameter(float arg_moveSpeed, float arg_rotateSpeed);
	private:
		void Rotate();
		void MoveBack();
		void StartMoveBack();
		void StartDance();
		void MoveHorizontal();

		void SpawnNewHead();

		void StopRotate();

		bool IsFrontHead();
		float GetDifferenceFromHeadFront();
		float GetLookForwardHeadAngle();
		void SaveFriendData();

		Vector3 GetFrontXZ(const Vector3& arg_front);

		void ResizeLevelParameter();

		Value_ptr<ButiRendering::IAnimationController> m_vlp_animationController;

		Value_weak_ptr<GameSettings> m_vwp_gameSettings;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;

		Value_weak_ptr<GameObject> m_vwp_head;
		Value_weak_ptr<GameObject> m_vwp_neck;

		Value_ptr<FriendData> m_vlp_friendData;

		std::vector<float> m_vec_rotateSpeeds;

		bool m_isRotate;
		bool m_isStopRotate;
		float m_frontBorder;

		bool m_isMoveBack;
		Value_ptr<RelativeTimer> m_vlp_moveBackTimer;
		Vector3 m_moveBackStartPos;
		Vector3 m_moveBackTargetPos;

		std::vector<std::int32_t> m_vec_moveHorizontalFrame;
		bool m_isMoveHorizontal;
		Value_ptr<RelativeTimer> m_vlp_moveHorizontalTimer;
		Vector3 m_moveHorizontalStartPos;
		Vector3 m_moveHorizontalTargetPos;

		Value_ptr<RelativeTimer> m_vlp_pullUpTimer;
		float m_moveSpeed;
		float m_rotateSpeed;
		Vector3 m_offsetPos;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendBody, true);