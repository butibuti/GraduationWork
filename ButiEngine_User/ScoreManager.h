#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
class Heart;
class ScoreManager :public GameComponent
{
public:

	std::string GetGameComponentName()const override {
		return "ScoreManager";
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

	std::int32_t GetScore() { return m_score; }

	void CalcScore();
private:
	std::int32_t m_score;
	Value_ptr<Heart> m_vlp_heart;
};

}

BUTI_REGIST_GAMECOMPONENT(ScoreManager, true);