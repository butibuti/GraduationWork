#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
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

	void CalcScore(Value_weak_ptr<GameObject> arg_vwp_head, Value_weak_ptr<GameObject> arg_vwp_body);
private:
	std::int32_t m_score;
};

}

BUTI_REGIST_GAMECOMPONENT(ScoreManager, true);