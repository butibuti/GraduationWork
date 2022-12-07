#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class StageManager;
	class PauseManager;
	class GameLevelManager;

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

			ARCHIVE_BUTI(m_spawnPosZ);

			ARCHIVE_BUTI(m_vec_maxFacePartCounts);
			ARCHIVE_BUTI(m_vec_minSpawnFacePartIntervalFrames);
			ARCHIVE_BUTI(m_vec_maxSpawnFacePartIntervalFrames);

			ARCHIVE_BUTI(m_vec_maxDummyPartCounts);
			ARCHIVE_BUTI(m_vec_minSpawnDummyPartIntervalFrames);
			ARCHIVE_BUTI(m_vec_maxSpawnDummyPartIntervalFrames);
		}

	private:
		void FirstSpawnFacePart();
		void SpawnFacePart();
		void SetSpawnFacePartInterval();
		void SpawnDummyPart();
		void SetSpawnDummyPartInterval();

		Vector3 GetRandomSpawnPartPos();
		std::string GetRandomSpawnPartName();

		bool CanUpdate();

		Value_weak_ptr<StageManager> m_vwp_stageManager;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;

		float m_spawnPosZ;

		Value_ptr<RelativeTimer> m_vlp_spawnFacePartTimer;
		std::vector<std::int32_t> m_vec_maxFacePartCounts;
		std::vector<std::int32_t> m_vec_minSpawnFacePartIntervalFrames;
		std::vector<std::int32_t> m_vec_maxSpawnFacePartIntervalFrames;

		Value_ptr<RelativeTimer> m_vlp_spawnDummyPartTimer;
		std::vector<std::int32_t> m_vec_maxDummyPartCounts;
		std::vector<std::int32_t> m_vec_minSpawnDummyPartIntervalFrames;
		std::vector<std::int32_t> m_vec_maxSpawnDummyPartIntervalFrames;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendFacePartSpawner, true);