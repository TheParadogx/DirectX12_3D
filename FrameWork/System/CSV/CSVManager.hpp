#pragma once
#include"CsvLoader.hpp"

namespace Engine::System
{
	class CSV
	{
	public:
		template<typename T>
		static CSVLoader<T>& Get()
		{
			static CSVLoader<T> Loader;
			return Loader;
		}
	};
}