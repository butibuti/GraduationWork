#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

	class GuideMarker :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "GuideMarker";
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

		void SetMarkTarget(Value_weak_ptr<GameObject> arg_markTarget) 
		{
			m_vwp_markTarget = arg_markTarget; 
			gameObject.lock()->transform->SetWorldPosition(m_vwp_markTarget.lock()->transform->GetWorldPosition());

			Vector3 screenPos = GetCamera("main")->WorldToScreen(m_vwp_markTarget.lock()->transform->GetWorldPosition());

			screenPos.z = gameObject.lock()->transform->GetLocalPosition().z;

			gameObject.lock()->transform->SetLocalPosition(screenPos);
		}
		void SetColor(const Vector4& arg_color) { gameObject.lock()->GetGameComponent<MeshDrawComponent>()->SetColor(arg_color); }
	private:
		Value_weak_ptr<GameObject> m_vwp_markTarget;

		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
	};

}

BUTI_REGIST_GAMECOMPONENT(GuideMarker, true);