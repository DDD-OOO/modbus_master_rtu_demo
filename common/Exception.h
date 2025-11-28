//
// Created by Eric Li on 25-5-12.
//

/*
 * @brief 异常类
 *
 * @author Eric Li
 * @date 2025-05-12
 */
#ifndef MODBUS_EXCEPTION_H
#define MODBUS_EXCEPTION_H

#include <exception>
#include <string>

namespace ems::modbus
{
	/*
	 * @brief modbus异常
	 */
	class Exception : public std::exception
	{
	public:
		~Exception() override = default;

		explicit Exception(std::string&& exp = "modbus exception")
			: message(exp)
		{
		}

		/*
		 * @brief 重写父类显示异常信息的函数
		 * @return const char*
		 */
		[[nodiscard]] const char* what() const noexcept override
		{
			return message.c_str();
		}

	protected:
		/*
		 * @brief 异常信息
		 */
		std::string message;
	};

	/**
	 * @brief 编码类型异常
	 */
	class EncodingTypeException : public Exception
	{
	public:
		EncodingTypeException()
			: Exception("unvalid encoding parameters")
		{}
	};

	/*
	 * @brief modbus类型异常
	 */
	class ConnectionTypeException : public Exception
	{
	public:
		explicit ConnectionTypeException(std::string&& exp)
			: Exception("unvalid modbus type: " + exp)
		{
		}
	};

	/**
	 * @brief RTU连接校验异常
	 */
	class RtuParityException : public Exception
	{
	public:
		explicit RtuParityException(std::string&& exp)
			: Exception("unvalid rtu parity: " + exp)
		{}
	};

	/**
	 * @brief 寄存器类型异常
	 */
	class RegisterTypeException : public Exception
	{
	public:
		explicit RegisterTypeException(std::string&& exp)
			: Exception("unvalid register type: " + exp)
		{}
	};

	/**
	 * @brief 寄存器数据类型异常
	 */
	class RegisterDataTypeException : public Exception
	{
	public:
		explicit RegisterDataTypeException(std::string&& exp)
			: Exception("unvalid register data type: " + exp)
		{}
	};

	/*
	 * @brief 建立连接异常
	 */
	class ConnectionEstablishingException : public Exception
	{
	public:
		explicit ConnectionEstablishingException(std::string&& exp)
			: Exception("connect failed: " + exp)
		{
		}
	};
}

#endif //MODBUS_EXCEPTION_H
