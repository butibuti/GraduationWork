#pragma once
#include"Header/GameComponentHeader.h"
#include"LevelEditor.h"
#include"FriendFacePart.h"
namespace ButiEngine {
class FacePartSpawner;
class PartRespawnPoint :public GameComponent
{
public:

	std::string GetGameComponentName()const override {
		return "PartRespawnPoint";
	}
	void OnUpdate()override;
	void Failed();
	void Success();
	void Damage();
	void Start()override;
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(isActive);
	}
	void SetSpawner(Value_weak_ptr<FacePartSpawner> arg_spawner) { m_vwp_spawner = arg_spawner; }
	void SetIsContinue(const bool arg_isContinue) { m_isContinue = arg_isContinue; }
	void SetParam(const FacePartParameter& arg_param) { m_param = arg_param; }
	const FacePartParameter& GetParam()const { return m_param; }
	void Clear();
private:
	void SpawnFacePart();
	Value_weak_ptr<FacePartSpawner> m_vwp_spawner;
	Value_weak_ptr<GameObject> m_vwp_part;
	bool m_isContinue=false;
	FacePartParameter m_param;
};

}

BUTI_REGIST_GAMECOMPONENT(PartRespawnPoint, true);