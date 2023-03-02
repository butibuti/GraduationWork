#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

class Effect_ExplosionNeedle :public GameComponent
{
public:
	std::string GetGameComponentName()const override {
		return "Effect_ExplosionNeedle";
	}
	void OnUpdate()override;
	void OnShowUI()override;
	void Start()override;
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(isActive);
		ARCHIVE_BUTI(m_initScale);
	}
private:
	float m_initScale;
	std::int32_t m_frame;
	List<Value_ptr<MeshDrawComponent>>m_list_vlp_mesh;
};

}

BUTI_REGIST_GAMECOMPONENT(Effect_ExplosionNeedle, true);