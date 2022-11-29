#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
class ScoreNumber :public GameComponent
{
public:

	std::string GetGameComponentName()const override {
		return "ScoreNumber";
	}
	void OnUpdate()override;
	void Start()override;
	void OnRemove()override;
	void OnShowUI()override;
	void SetNumber(const std::int8_t arg_num);
	void ChangeNumberMesh();
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(isActive);
	}
private:
	std::int8_t m_num=0,m_displayNum=0;
	Value_ptr<RelativeTimer>m_vlp_timer;
	bool m_isChanged = false;
	List<Value_ptr<MeshDrawComponent>> m_list_vlp_numMeshes;
};

}

BUTI_REGIST_GAMECOMPONENT(ScoreNumber, true);