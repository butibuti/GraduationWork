#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class GameSettings;
	class PauseManager;

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
		}

		void SetHead(Value_weak_ptr<GameObject> arg_vwp_head);
		std::int32_t GetScore();
	private:
		void Rotate();
		void MoveBack();
		void StartMoveBack();
		void SpawnNewHead();
		void SpawnNewBody();
		bool IsFront();

		Value_weak_ptr<GameSettings> m_vwp_gameSettings;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;

		float m_rotateSpeed;

		bool m_isRotate;
		bool m_isStopRotate;
		float m_frontBorder;

		bool m_isMoveBack;
		Value_ptr<RelativeTimer> m_vlp_moveTimer;
		Vector3 m_moveStartPos;
		Vector3 m_moveTargetPos;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendBody, true);