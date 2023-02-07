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
			ARCHIVE_BUTI(m_exactAngleBorder);
			ARCHIVE_BUTI(m_exactPosBorder);
		}

		void SetParent(Value_weak_ptr<GameObject> arg_vwp_parent)
		{
			m_vwp_parent = arg_vwp_parent; 
			gameObject.lock()->transform->SetBaseTransform(arg_vwp_parent.lock()->transform, true);
		}

		Value_weak_ptr<GameObject> GetParent() { return m_vwp_parent; }

		Value_weak_ptr<GameObject> GetStickPart() { return m_vwp_part; }

		bool CanStickPart(const PartType arg_type);

		void SetCanStickPart(const bool arg_canStickPart) { m_canStickPart = arg_canStickPart; }

		void StickPart(Value_weak_ptr<GameObject> arg_vwp_part);
		void OverWrite(Value_weak_ptr<GameObject> arg_vwp_part);
		void LeavePart();

		Vector3 GetStickPos();

		std::int32_t GetCalcScore();
		Value_weak_ptr<GameObject> GetPart() { return m_vwp_part; }

		void RemoveAllComponent();

		bool IsExact();
		bool IsExactAngle();
		bool IsExactPos();

		void Dead();
	private:
		void CreateGuideMarker();
		void SetDefaultPosObject();

		Value_weak_ptr<GameObject> m_vwp_parent;

		Value_weak_ptr<GameObject> m_vwp_part;

		Value_weak_ptr<GameObject> m_vwp_guideMarker;

		Value_weak_ptr<GameObject> m_vwp_defaultPosObject;

		PartType m_type;

		std::int32_t m_score;

		Vector3 m_standardPos;

		//ƒXƒRƒA‚ª0‚É‚È‚é‹——£
		float m_partFurthest;

		float m_exactAngleBorder;
		float m_exactPosBorder;

		bool m_canStickPart;
	};

}

BUTI_REGIST_GAMECOMPONENT(FriendHead_PartHitArea, true);