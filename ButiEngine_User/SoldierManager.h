#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SoldierManager :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "SoldierManager";
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

		std::vector<Value_weak_ptr<GameObject>> GetHomeSoldiers() { return m_vec_vwp_homeSoldiers; }
		Value_weak_ptr<GameObject> GetHomeSoldierRandom();
		std::int32_t GetHomeSoldierCount() { return m_vec_vwp_homeSoldiers.size(); }

		void AddHomeSoldier(Value_weak_ptr<GameObject> arg_soldier);
		void RemoveHomeSoldier(Value_weak_ptr<GameObject> arg_soldier);

	private:
		std::vector<Value_weak_ptr<GameObject>> m_vec_vwp_homeSoldiers;
	};

}

BUTI_REGIST_GAMECOMPONENT(SoldierManager, true);