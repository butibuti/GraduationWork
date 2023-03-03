#pragma once
#include"Header/GameComponentHeader.h"
#include"LevelEditor.h"
namespace ButiEngine {

class StageManager;
class PauseManager;
class GameLevelManager;
enum class MovePattern;

class FacePartSpawner :public GameComponent
{
public:

	std::string GetGameComponentName()const override {
		return "FacePartSpawner";
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
		ARCHIVE_BUTI(m_randomSelectLevelMin);
		ARCHIVE_BUTI(m_randomSelectLevelMax);
	}

	void Clear();
	void LevelIncrement();
	void SetLevel(const std::int32_t arg_level);
	void DecreaseParts() { m_remainPart--; }
	void Success() { m_successPart++; }
	void Damage() { m_successPart--; }
	void SetIsRandom(const bool arg_isRandom) { m_isRandomLevelSelect = arg_isRandom; }
	bool GetIsRandom ()const { return m_isRandomLevelSelect; }
	std::int32_t GetCurrentLevel()const { return m_currentEditLevelIndex; }
private:
	void CreatePartArrangement();
	void SpawnAccessory();

	Value_weak_ptr<StageManager> m_vwp_stageManager;
	Value_weak_ptr<PauseManager> m_vwp_pauseManager;
	Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;
	SceneData m_currentData;
	std::int32_t m_currentEditLevelIndex = -1,m_remainPart=0,m_successPart=0;
	bool m_isRandomLevelSelect=false;
	std::int32_t m_randomSelectLevelMin=0, m_randomSelectLevelMax=0;

	bool m_ExistDummyPart = false;
};

}

BUTI_REGIST_GAMECOMPONENT(FacePartSpawner, true);