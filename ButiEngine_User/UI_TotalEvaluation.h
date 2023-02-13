#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	enum class Rank;

	class UI_TotalEvaluation :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "UI_TotalEvaluation";
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

		void Appear();
		void Disappear();

		void SetTotalRank(const Rank arg_rank);
		void SetPartRank(const Rank arg_eyeRank, const Rank arg_noseRank, const Rank arg_mouthRank);
	private:
		void AddPositionAnimation(const float arg_targetPosX, Easing::EasingType arg_easeType, const std::int32_t arg_animFrame, const std::int32_t arg_index);

		std::vector<Value_weak_ptr<GameObject>> m_vec_vwp_texts;

		Value_ptr<RelativeTimer> m_vlp_appearIntervalTimer;
		Value_ptr<RelativeTimer> m_vlp_waitDisappearTimer;
		std::int32_t m_appearIndex;
	};

}

BUTI_REGIST_GAMECOMPONENT(UI_TotalEvaluation, true);