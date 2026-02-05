#pragma once
#include"System/CSV/CsvLoader.hpp"

namespace Engine::System
{

	struct SaveData
	{
		//	Ž¯•Ê—p
		int id = -1;
		//	0:–¢“¢”° 1:Â“¢”° 2:‰©“¢”° 3:Ô“¢”°
		int ClearLevel = 0;

		CSV_BINDABLE(SaveData, obj.id, obj.ClearLevel);
	};
}