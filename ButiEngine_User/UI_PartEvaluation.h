#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class NumberDraw;
	enum class Rank;

	class UI_PartEvaluation :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "UI_PartEvaluation";
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

		void SetScore(const std::int32_t arg_score) { m_score = arg_score; }
		void SetRank(const Rank arg_rank);
	private:
		void CheckLevelUp(const std::int32_t arg_score);
		void LevelUpNormal();
		void LevelUpGood();
		void SetColor(const Vector4& arg_color);
		void PlayRankSound();

		Value_weak_ptr<NumberDraw> m_vwp_numberDraw;


		std::int32_t m_score;
		Rank m_rank;
		Value_ptr<RelativeTimer> m_vlp_timer;
		Value_ptr<RelativeTimer> m_vlp_waitDisappearTimer;

		bool m_isLevelUpNormal;
		bool m_isLevelUpGood;
	};

}

BUTI_REGIST_GAMECOMPONENT(UI_PartEvaluation, true);