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

		void SetHead(Value_weak_ptr<GameObject> arg_head) { m_vwp_head = arg_head; }
		void SetBody(Value_weak_ptr<GameObject> arg_body) { m_vwp_body = arg_body; }
		void SetEye(Value_weak_ptr<GameObject> arg_eye) { m_vwp_head = arg_eye; }
		void SetNose(Value_weak_ptr<GameObject> arg_nose) { m_vwp_nose = arg_nose; }
		void SetMouth(Value_weak_ptr<GameObject> arg_mouth) { m_vwp_head = arg_mouth; }

		Value_weak_ptr<GameObject> GetHead() { return m_vwp_head; }
		Value_weak_ptr<GameObject> GetBody() { return m_vwp_body; }

		void CreateParts(Value_weak_ptr<FriendData> arg_vwp_friendData);
		void StartDance();
	private:
		void CreateHead(Value_weak_ptr<Transform> arg_vwp_transform);
		void CreateBody(Value_weak_ptr<Transform> arg_vwp_transform);
		void CreateEye(Value_weak_ptr<Transform> arg_vwp_transform, const Rank arg_rank);
		void CreateNose(Value_weak_ptr<Transform> arg_vwp_transform, const Rank arg_rank);
		void CreateMouth(Value_weak_ptr<Transform> arg_vwp_transform, const Rank arg_rank);

		Value_weak_ptr<GameObject> m_vwp_head;
		Value_weak_ptr<GameObject> m_vwp_body;
		Value_weak_ptr<GameObject> m_vwp_eye;
		Value_weak_ptr<GameObject> m_vwp_nose;
		Value_weak_ptr<GameObject> m_vwp_mouth;

		Value_ptr<ButiRendering::IAnimationController> m_vlp_animationController;

		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
	};

}

BUTI_REGIST_GAMECOMPONENT(CompleteFriend, true);