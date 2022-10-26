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
			ARCHIVE_BUTI(m_maxScore);
		}

		std::int32_t GetScore() { return m_score; }

		void AddScore(const std::int32_t arg_addScore);
		void RemoveScore(const std::int32_t arg_removeScore);
	private:
		std::int32_t m_maxScore = 1000;
		std::int32_t m_score;
	};

}

BUTI_REGIST_GAMECOMPONENT(ScoreManager, true);