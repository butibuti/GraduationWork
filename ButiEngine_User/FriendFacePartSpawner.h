#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class FriendFacePartSpawner :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FriendFacePartSpawner";
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
			ARCHIVE_BUTI(m_maxFacePartCount);
			ARCHIVE_BUTI(m_minSpawnIntervalFrame);
			ARCHIVE_BUTI(m_maxSpawnIntervalFrame);
		}

	private:
		void SpawnFacePart();
		void SetSpawnInterval();

		std::int32_t m_maxFacePartCount;
		std::int32_t m_minSpawnIntervalFrame;
		std::int32_t m_maxSpawnIntervalFrame;

		Value_ptr<RelativeTimer> m_vlp_spawnTimer;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendFacePartSpawner, true);