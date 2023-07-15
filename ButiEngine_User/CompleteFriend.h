#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	struct FriendData;
	class PauseManager;
	enum class Rank;

	class CompleteFriend :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "CompleteFriend";
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

		void SetFriendData(Value_weak_ptr<FriendData> arg_friendData) { m_vwp_friendData = arg_friendData; }
		void SetHead(Value_weak_ptr<GameObject> arg_head) { m_vwp_head = arg_head; }
		void SetBody(Value_weak_ptr<GameObject> arg_body);
		void SetHeart(Value_weak_ptr<GameObject> arg_heart) { m_vwp_heart = arg_heart; }
		void SetEye(Value_weak_ptr<GameObject> arg_eye) { m_vwp_eye = arg_eye; }
		void SetNose(Value_weak_ptr<GameObject> arg_nose) { m_vwp_nose = arg_nose; }
		void SetMouth(Value_weak_ptr<GameObject> arg_mouth) { m_vwp_mouth = arg_mouth; }
		void SetHelmet(Value_weak_ptr<GameObject> arg_helmet) { m_vwp_helmet = arg_helmet; }
		void SetChara(Value_weak_ptr<GameObject> arg_chara) { m_vwp_chara = arg_chara; }

		Value_weak_ptr<GameObject> GetHead() { return m_vwp_head; }
		Value_weak_ptr<GameObject> GetBody() { return m_vwp_body; }

		void Dead(bool arg_isRemoveCompleteFirnedList = false, bool arg_isAddTargetPosIndex = false);

		void CreateParts(Value_weak_ptr<FriendData> arg_vwp_friendData);
		void StartDance();
		void StopDance();
		void StartBlowAnimation();

		void SetTargetPosIndex(const std::int16_t arg_index) { m_targetPosIndex = arg_index; }

		void ResetStartColor();
		void SetBrightness(const float arg_brightness);
	private:
		void CreateHead(Value_weak_ptr<Transform> arg_vwp_transform);
		void CreateBody(Value_weak_ptr<Transform> arg_vwp_transform);
		void CreateEye(Value_weak_ptr<Transform> arg_vwp_transform, const Rank arg_rank);
		void CreateNose(Value_weak_ptr<Transform> arg_vwp_transform, const Rank arg_rank);
		void CreateMouth(Value_weak_ptr<Transform> arg_vwp_transform, const Rank arg_rank);
		void CreateHelmet();
		void CreateChara();

		Value_weak_ptr<GameObject> m_vwp_head;
		Value_weak_ptr<GameObject> m_vwp_body;
		Value_weak_ptr<GameObject> m_vwp_heart;
		Value_weak_ptr<GameObject> m_vwp_eye;
		Value_weak_ptr<GameObject> m_vwp_nose;
		Value_weak_ptr<GameObject> m_vwp_mouth;
		Value_weak_ptr<GameObject> m_vwp_helmet;
		Value_weak_ptr<GameObject> m_vwp_chara;

		Value_weak_ptr<FriendData> m_vwp_friendData;

		Value_ptr<ButiRendering::IAnimationController> m_vlp_animationController;
		bool m_isDance = false;
		bool m_isBlow = false;

		Value_weak_ptr<PauseManager> m_vwp_pauseManager;

		std::uint16_t m_targetPosIndex = 0;
	};

}

BUTI_REGIST_GAMECOMPONENT(CompleteFriend, true);