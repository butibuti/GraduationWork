#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	enum class PartType;

	class FriendHead_PartHitArea :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FriendHead_PartHitArea";
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
			ARCHIVE_BUTI(m_type);
			ARCHIVE_BUTI(m_partFurthest);
		}

		void SetHead(Value_weak_ptr<GameObject> arg_vwp_head) { m_vwp_head = arg_vwp_head; }

		Value_weak_ptr<GameObject> GetStickPart() { return m_vwp_part; }
		std::int32_t GetDummyPartCount() { return m_vec_vwp_dummyParts.size(); }

		bool CanStickPart(const PartType arg_type);
		void StickPart(Value_weak_ptr<GameObject> arg_vwp_part, const PartType arg_type);

		std::int32_t GetCalcScore();

		void RemoveAllComponent();
	private:
		Value_weak_ptr<GameObject> m_vwp_head;

		Value_weak_ptr<GameObject> m_vwp_part;
		std::vector<Value_weak_ptr<GameObject>> m_vec_vwp_dummyParts;

		PartType m_type;

		std::int32_t m_score;

		//ƒXƒRƒA‚ª0‚É‚È‚é‹——£
		float m_partFurthest;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendHead_PartHitArea, true);