#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
class ScoreNumber;
class Heart :public GameComponent
{
public:

	std::string GetGameComponentName()const override {
		return "Heart";
	}
	void OnUpdate()override;
	void OnSet()override;
	void Start()override;
	void OnRemove()override;
	void OnShowUI()override;
	void SetScore(const std::int32_t arg_score);
	Value_ptr<GameComponent> Clone()override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(isActive);
		ARCHIVE_BUTI(m_maxDigit);
		ARCHIVE_BUTI(m_minHeartSize);
		ARCHIVE_BUTI(m_maxHeartSize);
		ARCHIVE_BUTI(m_rotationSpeed);
		ARCHIVE_BUTI(m_letterSpace);
	}
private:
	std::int32_t m_maxDigit=6,m_currentScore=0,m_displayingScore=0,m_maxScore=0;
	List<Value_ptr<ScoreNumber>> m_list_vlp_scoreNumbers;
	float m_minHeartSize=0,m_maxHeartSize=0,m_rotationSpeed=1.0f,m_letterSpace=0.0f;
};

}

BUTI_REGIST_GAMECOMPONENT(Heart, true);