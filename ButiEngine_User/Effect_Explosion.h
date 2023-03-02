#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

class Effect_Explosion :public GameComponent
{
public:
	std::string GetGameComponentName()const override {
		return "Effect_Explosion";
	}
	void OnUpdate()override;
	void OnShowUI()override;
	void Start()override;
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(isActive);
	}
private:
	Value_ptr<GameObject> m_vlp_smokeParent, m_vlp_needleParent;
	Value_ptr<RelativeTimer> m_vlp_life;
};

}

BUTI_REGIST_GAMECOMPONENT(Effect_Explosion, true);