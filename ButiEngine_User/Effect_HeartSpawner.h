#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Effect_HeartSpawner :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Effect_HeartSpawner";
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
	private:
		void SpawnHeart();
		void SetSpawnInterval();

		Value_ptr<RelativeTimer> m_vlp_spawnTimer;
		std::int32_t m_minSpawnIntervalFrame;
		std::int32_t m_maxSpawnIntervalFrame;
	};

}

BUTI_REGIST_GAMECOMPONENT(Effect_HeartSpawner, true);