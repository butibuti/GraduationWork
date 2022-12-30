#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Effect_ConcentratedLine :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Effect_ConcentratedLine";
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
			ARCHIVE_BUTI(m_lifeTime);
			ARCHIVE_BUTI(m_rotateSpeed);
		}

		void SetLifeTime(const std::int32_t arg_lifeTime) { m_lifeTime = arg_lifeTime; }
	private:
		Value_weak_ptr<GameObject> m_vwp_parent;
		std::int32_t m_lifeTime;
		float m_rotateSpeed;
		Value_ptr<RelativeTimer> m_vlp_lifeTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(Effect_ConcentratedLine, true);