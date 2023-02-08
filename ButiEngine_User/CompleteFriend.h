#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	struct FriendData;
	class PauseManager;

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

		Value_weak_ptr<GameObject> GetHead() { return m_vwp_head; }
		Value_weak_ptr<GameObject> GetBody() { return m_vwp_body; }

		void CreateParts(Value_weak_ptr<FriendData> arg_vwp_friendData);
		void StartDance();
	private:
		Value_weak_ptr<GameObject> m_vwp_head;
		Value_weak_ptr<GameObject> m_vwp_body;
		Value_ptr<ButiRendering::IAnimationController> m_vlp_animationController;

		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
	};

}

BUTI_REGIST_GAMECOMPONENT(CompleteFriend, true);