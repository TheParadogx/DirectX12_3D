#pragma once

#include "Graphics/SkyBox/Resource/SkyBoxResource.hpp"
#include "Graphics/ConstantBuffer/ConstantBuffer.hpp"
#include "Math/Matrix/Matrix.h"

#include<memory>

namespace Engine::Graphics
{
	class SkyBox
	{
	public:
		SkyBox();
		virtual ~SkyBox();

		/// <summary>
		/// 作成
		/// </summary>
		/// <param name="Resource">リソース管理から取得したリソースのポインタ</param>
		/// <returns>true:成功</returns>
		bool Create(SkyBoxResource* Resource);

		/// <summary>
		/// 解放
		/// </summary>
		void Release();

		/// <summary>
		/// 描画
		/// </summary>
		void Render();

	private:
		struct SkyBoxConstantBufferInfo
		{
			Math::Matrix World; //	パティング用
			Math::Matrix View;
			Math::Matrix Projection;
		};

		//	255アライメントに合わせます
		static constexpr auto ConstantBufferSize = (sizeof(SkyBoxConstantBufferInfo) + 255) & ~255;

	private:
		/// <summary>
		/// リソースの参照
		/// </summary>
		SkyBoxResource* mResource;

		/// <summary>
		/// 行列用
		/// </summary>
		std::unique_ptr<ConstantBuffer> mTransformBuffer;

	};
}

