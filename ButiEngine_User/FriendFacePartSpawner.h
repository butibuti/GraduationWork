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
			ARCHIVE_BUTI(m_vec_maxFacePartCounts);
			ARCHIVE_BUTI(m_vec_minSpawnIntervalFrames);
			ARCHIVE_BUTI(m_vec_maxSpawnIntervalFrames);
			ARCHIVE_BUTI(m_spawnPosZ);
		}

	private:
		void FirstSpawnFacePart();
		void SpawnFacePart();
		void SetSpawnInterval();

		Vector3 GetRandomSpawnPartPos();
		std::string GetRandomSpawnPartName();

		bool CanUpdate();

		Value_weak_ptr<StageManager> m_vwp_stageManager;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;

		std::vector<std::int32_t> m_vec_maxFacePartCounts;
		std::vector<std::int32_t> m_vec_minSpawnIntervalFrames;
		std::vector<std::int32_t> m_vec_maxSpawnIntervalFrames;

		Value_ptr<RelativeTimer> m_vlp_spawnTimer;

		float m_spawnPosZ;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendFacePartSpawner, true);