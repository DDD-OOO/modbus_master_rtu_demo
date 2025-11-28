//
// Created by Eric Li on 25-5-12.
//
//

/*
 * @brief modbus配置类
 *
 * @author Eric Li
 * @date 2025-05-12
 */
#ifndef EMS_MODBUS_CONFIG_H
#define EMS_MODBUS_CONFIG_H

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <variant>

#include "Util.h"

namespace ems::modbus
{
	/*
	 * @brief 连接类型枚举, 暂时只支持RTU和TCP
	 *
	 */
	enum class ConnectionType
	{
		TCP,
		RTU
	};

	template<>
	struct EnumTraits<ConnectionType>
	{
		struct Pair
		{
			ConnectionType value;
			std::string_view name;
		};

		static constexpr std::array<Pair, 2> pairs = {
			{
				{ConnectionType::TCP, "tcp"},
				{ConnectionType::RTU, "rtu"},
			}};
	};

	/*
	 * @brief TCP连接配置
	 */
	struct TcpConfig
	{
		/*
		 * @bref 端口
		 */
		int port;

		/*
		 * @bref IP地址
		 */
		std::string ip;
	};

	/*
	 * @brief RTU连接配置
	 */
	struct RtuConfig
	{
		/*
		 * @brief 校验位
		 */
		char parity;

		/*
		 * @brief 波特率
		 */
		int baud_rate;

		/*
		 * @brief 数据位
		 */
		int data_bits;

		/*
		 * @brief 停止位
		 */
		int stop_bits;

		/*
		 * @brief 串口
		 */
		std::string serial_port;
	};

	/*
	 * @brief 连接配置模板类, 约束该类型只能是TCP连接配置类或RTU连接配置类
	 */
	template <typename T>
	concept ConnectionConfig = std::is_same_v<T, TcpConfig> || std::is_same_v<T, RtuConfig>;

	/*
	 * @brief Linux类型的超时类
	 */
	struct Timeout
	{
		/*
		 * @brief 秒
		 */
		long sec;

		/*
		 * @brief 微妙
		 */
		int usec;
	};

	/*
	* @brief 字节序
	*/
	enum class Endianness
	{
		/*
		 * @brief 大端序列, 如AB
		 */
		BIG,

		/*
		 * @brief 小端序列, 如BA
		 */
		LITTLE
	};

	template<>
	struct EnumTraits<Endianness>
	{
		struct Pair
		{
			Endianness value;
			std::string_view name;
		};

		static constexpr std::array<Pair, 2> pairs = {
			{
				{Endianness::BIG, "big"},
				{Endianness::LITTLE, "little"}
			}};
	};

	/*
	* @brief 编码
	*/
	struct Encoding
	{
		/*
		* @brief 词序是否需要交换, 如大端的ABCD交换之后就是BACD, 小端的DCBA交换之后就是CDAB
		*/
		bool byte_swap;

		/*
		* @brief 字节序
		*/
		Endianness endianness;
	};


	/*
	 * @brief 配置类
	 */
	template <ConnectionConfig T>
	struct Config
	{
		/*
		 * 连接类型
		 */
		ConnectionType connection_type{ConnectionType::TCP};

		/*
		 * @brief 超时
		 */
		Timeout timeout{};

		/*
		 * @brief 连接配置
		 */
		T connection_config;

		/*
		 * @brief 编码
		 */
		Encoding encoding{};
	};

	using ActualData = std::variant<std::monostate, bool, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t, float, double, std::string>;

	enum class RegisterType : uint8_t
	{
		COIL = 0,
		DISCRETE_INPUT = 1,
		HOLDING_REGISTER = 2,
		INPUT_REGISTER = 3
	};

	template<>
	struct EnumTraits<RegisterType>
	{
		struct Pair
		{
			RegisterType value;
			std::string_view name;
		};

		static constexpr std::array<Pair, 4> pairs = {
			{
				{RegisterType::COIL, "coil"},
				{RegisterType::DISCRETE_INPUT, "discrete_input"},
				{RegisterType::HOLDING_REGISTER, "holding_register"},
				{RegisterType::INPUT_REGISTER, "input_register"}
			}};
	};

	enum class DataType : uint8_t
	{
		BOOL = 0,
		UINT16 = 1,
		INT16 = 2,
		STRING = 3,
		UINT32 = 4,
		INT32 = 5,
		FLOAT = 6,
		UINT64 = 7,
		INT64 = 8,
		DOUBLE = 9,
	};

	template<>
	struct EnumTraits<DataType>
	{
		struct Pair
		{
			DataType value;
			std::string_view name;
		};

		static constexpr std::array<Pair, 10> pairs = {
			{
				{DataType::BOOL, "bool"},
				{DataType::UINT16, "uint16"},
				{DataType::INT16, "int16"},
				{DataType::STRING, "string"},
				{DataType::UINT32, "uint32"},
				{DataType::INT32, "int32"},
				{DataType::FLOAT, "float"},
				{DataType::UINT64, "uint64"},
				{DataType::INT64, "int64"},
				{DataType::DOUBLE, "double"}
			}};
	};

	struct Register
	{
		RegisterType register_type;
		DataType register_data_type;
		uint16_t address;
		int number;
		std::optional<int> offset;
		std::optional<double> accuracy;
	};
}

#endif //EMS_MODBUS_CONFIG_H
