//
// Created by Eric Li on 25-7-18.
//

#ifndef EMS_MODBUS_UTIL_H
#define EMS_MODBUS_UTIL_H

#include <optional>
#include <string>

namespace ems::modbus
{
	class Util
	{
	public:
		~Util() = default;

		static std::string transform_to_lower(const std::string& data);
	};

	template<typename Enum>
	struct EnumTraits;

	template<typename Enum>
	class EnumConverter
	{
	public:
		static constexpr std::string_view to_string(Enum value)
		{
			constexpr auto pairs = EnumTraits<Enum>::pairs;
			for (const auto& pair : pairs)
			{
				if (pair.value == value)
					return pair.name;
			}

			return "Unknown";
		}

		static constexpr std::optional<Enum> from_string(std::string_view str)
		{
			constexpr auto pairs = EnumTraits<Enum>::pairs;
			for (const auto& pair : pairs)
			{
				if (pair.name == str)
					return pair.value;
			}
			return std::nullopt;
		}

		static constexpr auto get_all_values()
		{
			return EnumTraits<Enum>::pairs;
		}
	};
}

#endif //EMS_MODBUS_UTIL_H
