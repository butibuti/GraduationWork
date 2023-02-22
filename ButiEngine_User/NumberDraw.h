#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpriteAnimationComponent;

	class NumberDraw :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "NumberDraw";
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
			ARCHIVE_BUTI(m_digitCount);
			ARCHIVE_BUTI(m_isFillZero);
		}

		void SetNumber(const std::int32_t arg_number);
		void SetColor(const Vector4& arg_color);
		void SetMeshDrawComponent();
	private:
		void SetHideDigit(const std::int32_t arg_number, std::vector<std::int32_t> arg_vec_digitNums);
		std::vector<std::int32_t> GetDigitNums(const std::int32_t arg_number);

		std::vector<Value_weak_ptr<MeshDrawComponent>> m_vec_vwp_meshDrawComponents;
		std::vector<Value_weak_ptr<SpriteAnimationComponent>> m_vec_vwp_spriteAnimationComponents;

		Vector3 m_defaultScale;

		std::int32_t m_digitCount = 3;
		bool m_isFillZero = false;

		bool m_isInitialized = false;
	};

}

BUTI_REGIST_GAMECOMPONENT(NumberDraw, true);