//
// Created by Eric Li on 25-5-24.
//

#ifndef MODBUS_MASTER_EXCEPTION_H
#define MODBUS_MASTER_EXCEPTION_H

#include <string>

#include "common/Exception.h"

namespace ems::modbus::master
{
	/**
	 * @brief 通信错误
	 */
	class CommunicationException : public Exception
	{
	public:
		explicit CommunicationException(std::string&& exp)
			: Exception("communication failed: " + exp)
		{
		}
	};

	class NonSocketException : public Exception
	{
	public:
		explicit NonSocketException(std::string&& exp)
			: Exception("non socket error: " + exp)
		{
		}
	};
}

#endif //MODBUS_MASTER_EXCEPTION_H
