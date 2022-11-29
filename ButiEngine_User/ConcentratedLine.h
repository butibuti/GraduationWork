#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ConcentratedLine :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "ConcentratedLine";
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

		void SetLifeTime(const std::int32_t arg_lifeTime) { m_lifeTime = arg_lifeTime; }
	private:
		Value_weak_ptr<GameObject> m_vwp_parent;
		std::int32_t m_lifeTime;
		Value_ptr<RelativeTimer> m_vlp_lifeTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(ConcentratedLine, true);