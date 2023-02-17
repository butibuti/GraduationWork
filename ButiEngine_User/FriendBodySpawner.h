#pragma once
#include"Header/GameComponentHeader.h"
#include"LevelEditor.h"
namespace ButiEngine {

class StageManager;
class PauseManager;
class GameLevelManager;
class FacePartSpawner;
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
	void DecreaceBodiesNumber();
	std::int32_t GetBodiesNumber() { return m_existingBodiesNumber; }
private:
	void SpawnBody();
	void SpawnBody(Vector3 spawnPosition, float moveSpeed, float rotateSpeed, float initRotationY);
	void SpawnBombBody(Vector3 spawnPosition, float moveSpeed, float rotateSpeed, float initRotationY, std::int32_t arg_bombCount);

	void StartSpawnPattern();
	void UpdateSpawnPattern();

	void StartNextSpawnPattern();

	void SetSpawnInterval();

	bool CanUpdate();
	void ResizeLevelParameter();

	Value_weak_ptr<StageManager> m_vwp_stageManager;
	Value_weak_ptr<PauseManager> m_vwp_pauseManager;
	Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;

	std::vector<std::int32_t> m_vec_spawnIntervalFrames;
	Value_ptr<RelativeTimer> m_vlp_spawnTimer;

	std::int32_t m_currentSpawnPatternTime;
	std::int32_t m_spawnedBodiesNumber;
	std::int32_t m_existingBodiesNumber;
	Value_ptr<RelativeTimer> m_vlp_spawnPatternTimer;

	std::int32_t m_spawnPatternOrderCounter;
	bool m_isPlayPattern;
	Value_weak_ptr<FacePartSpawner> m_vwp_facePartSpawner;
	SceneData m_currentData;
};
}

BUTI_REGIST_GAMECOMPONENT(FriendBodySpawner, true);