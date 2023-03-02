#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

class Effect_ExplosionSmoke :public GameComponent
{
public:
	std::string GetGameComponentName()const override {
		return "Effect_ExplosionSmoke";
	}
	void OnUpdate()override;
	void OnShowUI()override;
	void Start()override;
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(isActive);
		ARCHIVE_BUTI(m_initSmokeScale);
		ARCHIVE_BUTI(m_targetSmokeScale);
		ARCHIVE_BUTI(m_minSmokeDistance);
		ARCHIVE_BUTI(m_maxSmokeDistance);
	}
private:
	float m_initSmokeScale, m_targetSmokeScale, m_minSmokeDistance,m_maxSmokeDistance;
	std::int32_t m_frame;
	List<Value_ptr<MeshDrawComponent>>m_list_vlp_mesh;
};

}

BUTI_REGIST_GAMECOMPONENT(Effect_ExplosionSmoke, true);