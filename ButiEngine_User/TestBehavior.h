#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
class TestBehavior :public GameComponent
{
public:

	std::string GetGameComponentName()const override {
		return "TestBehavior";
	}
	void OnUpdate()override;
	void OnSet()override;
	void Start()override;
	void OnRemove()override;
	void OnShowUI()override;
	void ShowGUI()override;
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(isActive);
		ARCHIVE_BUTI(m_index);
	}
private:
	std::int32_t m_index;
};

}

BUTI_REGIST_GAMECOMPONENT(TestBehavior,true);