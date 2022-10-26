#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SoldierSpawner :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "SoldierSpawner";
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
			ARCHIVE_BUTI(m_maxSoldierCount);
			ARCHIVE_BUTI(m_spawnIntervalFrame);
		}

		void Dead();
	private:
		void SpawnSoldier();

		std::int32_t m_maxSoldierCount = 10;
		std::int32_t m_spawnIntervalFrame = 180;
		Value_ptr<RelativeTimer> m_vlp_spawnIntervalTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(SoldierSpawner, true);