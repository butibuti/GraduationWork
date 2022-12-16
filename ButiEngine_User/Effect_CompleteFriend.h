#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Effect_CompleteFriend :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Effect_CompleteFriend";
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
			ARCHIVE_BUTI(m_animationFrame);
		}

		void SetBody(Value_weak_ptr<GameObject> arg_vwp_body) { m_vwp_body = arg_vwp_body; }
	private:
		void CreateBeams();
		void Dead();
		std::vector<Value_weak_ptr<GameObject>> m_vec_vwp_beams;

		std::int32_t m_animationFrame;
		Value_ptr<RelativeTimer> m_vlp_animationTimer;

		Value_weak_ptr<GameObject> m_vwp_body;
		Value_weak_ptr<GameObject> m_vwp_parent;
	};

}

BUTI_REGIST_GAMECOMPONENT(Effect_CompleteFriend, true);