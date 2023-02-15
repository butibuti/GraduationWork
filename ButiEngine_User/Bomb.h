#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Bomb :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Bomb";
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

		void SetFrameToExplode(const std::int32_t arg_frame) { m_frameToExplode = arg_frame; }
		void SetParent(Value_weak_ptr<GameObject> arg_parent) { m_vwp_parent = arg_parent; }

		void Dead();
		void StartFall();
	private:
		void Explode();
		void ChangeColor();
		void ScaleAnimation();
		void Fall();

		Value_weak_ptr<GameObject> m_vwp_parent;

		Value_ptr<RelativeTimer> m_vlp_explodeTimer;
		std::int32_t m_frameToExplode;

		Value_ptr<RelativeTimer> m_vlp_waitExplodeTimer;

		Value_weak_ptr<MeshDrawComponent> m_vwp_meshDraw;

		float m_startScale;

		bool m_isFall;
	};

}

BUTI_REGIST_GAMECOMPONENT(Bomb, true);