 //
// Created by Eric Li on 25-7-18.
//
#include <algorithm>

#include "Util.h"

namespace ems::modbus
{
	std::string Util::transform_to_lower(const std::string& data)
	{
		std::string ret = data;
		std::ranges::transform(ret.begin(), ret.end(), ret.begin(), tolower);
		return ret;
	}
}