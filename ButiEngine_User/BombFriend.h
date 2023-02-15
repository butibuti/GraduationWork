#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class BombFriend :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "BombFriend";
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

		void Dead();
	private:
		void CreateBomb();

		Value_ptr<RelativeTimer> m_vlp_explodeTimer;
		std::int32_t m_frameToExplode;

		Value_weak_ptr<GameObject> m_vwp_bomb;
	};

}

BUTI_REGIST_GAMECOMPONENT(BombFriend, true);