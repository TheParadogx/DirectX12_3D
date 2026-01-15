#pragma once

namespace Engine::System
{
	class Memory
	{
	public:
		/// <summary>
		/// KiB‚ðŒvŽZ
		/// </summary>
		/// <param name="Size"></param>
		/// <returns></returns>
		static constexpr size_t KiB(const size_t Size)
		{
			return Size * 1024;
		}

		/// <summary>
		/// MiB‚ðŒvŽZ
		/// </summary>
		/// <param name="Size"></param>
		/// <returns></returns>
		static constexpr size_t MiB(const size_t Size)
		{
			return KiB(Size) * 1024;
		}

		/// <summary>
		/// GiB‚ðŒvŽZ
		/// </summary>
		/// <param name="Size"></param>
		/// <returns></returns>
		static constexpr size_t GiB(const size_t Size)
		{
			return MiB(Size) * 1024;
		}
	};
}