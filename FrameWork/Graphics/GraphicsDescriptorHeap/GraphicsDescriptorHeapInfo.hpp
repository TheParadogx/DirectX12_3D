#pragma once

namespace Engine::Graphics
{
	struct DescriptorHeapInfo
	{
		int index;
		int size;

		/// <summary>
		/// ”’l‚ª”ÍˆÍ“à‚©‚Ç‚¤‚©
		/// </summary>
		/// <returns>true:³í’l</returns>
		bool IsValid() const noexcept
		{
			return index >= 0 && size > 0;
		}
	};
}
