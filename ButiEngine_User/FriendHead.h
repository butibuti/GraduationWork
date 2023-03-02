#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class InputManager;
	class GameSettings;
	class RigidBodyComponent;
	class PauseManager;
	class FriendManager;
	class FriendHead_PartHitArea;
	class FriendHead_Center;
	class StageManager;

	class FriendHead :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FriendHead";
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
			ARCHIVE_BUTI(m_trackerIndex);
		}

		Value_weak_ptr<GameObject> GetEye();
		Value_weak_ptr<GameObject> GetNose();
		Value_weak_ptr<GameObject> GetMouth();
		Value_weak_ptr<GameObject> GetHelmet() { return m_vwp_helmet; }

		void SetHelmet(Value_weak_ptr<GameObject> arg_vwp_helmet)
		{
			auto sound = gameObject.lock()->GetResourceContainer()->GetSound(SoundTag("Sound/GetHelmet.wav"));
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(sound, 0.5f);
			m_vwp_helmet = arg_vwp_helmet; 
			m_vwp_helmet.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
		}

		bool IsGood();
		bool ExistPartStuckArea();
		void LeavePartRandom();

		void Dead();
		void Disappear();
	private:
		void Control();
		void ControlByKeyboard();
		void ControlByGamePad();
		void ControlByVRTracker();
		void OnPut(Value_weak_ptr<GameObject> arg_vwp_body);
		void CompleteFace();

		void Appear();
		void OnDisappear();
		void AddScaleAnimation(const Vector3& arg_targetScale, Easing::EasingType arg_easeType);
		void CheckPut();
		bool CanPut();
		bool CanUpdate();
		std::vector<Value_weak_ptr<FriendHead_PartHitArea>> GetPartStuckAreas();

		void CreatePartHitArea();
		void RemoveTriggerComponent();

		Value_weak_ptr<StageManager> m_vwp_stageManager;
		Value_weak_ptr<InputManager> m_vwp_inputManager;
		Value_weak_ptr<GameSettings> m_vwp_gameSettings;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<FriendManager> m_vwp_friendManager;

		std::int32_t m_trackerIndex;

		Value_weak_ptr<GameObject> m_vwp_headCenter;
		Value_weak_ptr<FriendHead_Center> m_vwp_headCenterComponent;

		//各パーツがくっつく範囲
		Value_weak_ptr<GameObject> m_vwp_eyesHitArea;
		Value_weak_ptr<GameObject> m_vwp_noseHitArea;
		Value_weak_ptr<GameObject> m_vwp_mouthHitArea;
		Value_weak_ptr<GameObject> m_vwp_otherHitArea;

		Value_weak_ptr<FriendHead_PartHitArea> m_vwp_eyesHitAreaComponent;
		Value_weak_ptr<FriendHead_PartHitArea> m_vwp_noseHitAreaComponent;
		Value_weak_ptr<FriendHead_PartHitArea> m_vwp_mouthHitAreaComponent;
		Value_weak_ptr<FriendHead_PartHitArea> m_vwp_otherHitAreaComponent;

		Value_weak_ptr<GameObject> m_vwp_helmet;

		Value_ptr<RelativeTimer> m_vlp_appearTimer;

		bool m_isPut;
		bool m_isCompleteFace;
		bool m_isShowGuide;
		bool m_isDisappear;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendHead, true);