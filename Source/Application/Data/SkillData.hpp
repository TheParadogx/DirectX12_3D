#pragma once

#include"System/CSV/CsvLoader.hpp"

namespace Engine::System
{
	struct SkillData
	{
		int id = 0;
		float Attack = 0;

		CSV_BINDABLE(SkillData, obj.id, obj.Attack);
	};
}