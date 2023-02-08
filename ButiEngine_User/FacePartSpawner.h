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
	}

	void Clear();
	void LevelIncrement();
	void DecreaseParts() { m_remainPart--; }
private:
	void CreatePartArrangement();

	Value_weak_ptr<StageManager> m_vwp_stageManager;
	Value_weak_ptr<PauseManager> m_vwp_pauseManager;
	Value_weak_ptr<GameLevelManager> m_vwp_gameLevelManager;
	SceneData m_currentData;
	std::int32_t m_currentEditLevelIndex = -1,m_remainPart=0,m_successPart=0;
};

}

BUTI_REGIST_GAMECOMPONENT(FacePartSpawner, true);