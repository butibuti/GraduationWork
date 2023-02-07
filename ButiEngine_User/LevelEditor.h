#pragma once
#include"Header/GameComponentHeader.h"
#include"FriendFacePart.h"
namespace ButiEngine {


struct PartArrangeData {
	FacePartParameter faceParam;
	Value_ptr<Transform> transform;

	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(faceParam);
		ARCHIVE_BUTI(transform);
	}
};
struct LevelData {
	List<PartArrangeData> list_data;
	bool isContinue = false;
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(list_data);
		ARCHIVE_BUTI(isContinue);
	}
};
struct SceneData {
	List<LevelData> list_data;

	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(list_data);
	}
};
class LevelEditor :public GameComponent
{
public:

	std::string GetGameComponentName()const override {
		return "LevelEditor";
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
	void Save();
	void CreatePreview();
	void ReleasePreview();
	void RecreatePreview();
	SceneData m_currentData;
	std::int32_t m_currentEditLevelIndex=-1;
};

}

BUTI_REGIST_GAMECOMPONENT(LevelEditor, true);