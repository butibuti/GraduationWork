#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class StageManager;
	class PauseManager;
	class GameLevelManager;

	class FriendBodySpawner :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FriendBodySpawner";
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
			ARCHIVE_BUTI(m_vec_spawnIntervalFrames);
		}
	private:
		void SpawnBody();

		void SetSpawnInterval();

		bool CanUpdate();
		void ResizeLevelParameter();

		Value_weak_ptr<StageManager> m_vwp_stageManager;
		Value_weak_ptr<PauseManager> m_vwp_pauseManager;
		Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;

		std::vector<std::int32_t> m_vec_spawnIntervalFrames;
		Value_ptr<RelativeTimer> m_vlp_spawnTimer;

	};
}

BUTI_REGIST_GAMECOMPONENT(FriendBodySpawner, true);