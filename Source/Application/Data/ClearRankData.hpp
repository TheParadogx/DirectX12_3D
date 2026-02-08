#pragma once
#include"System/CSV/CsvLoader.hpp"

#include<string>
namespace Engine::System
{

	struct ClearRankData
	{
		int id = -1;
		double ClearTime = 0;
		std::string Rank = "";

		CSV_BINDABLE(ClearRankData, obj.id, obj.ClearTime, obj.Rank);
	};
}