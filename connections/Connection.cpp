//
// Created by Eric Li on 25-5-23.
//

#include <cstring>

#include "Connection.h"
#include "exceptions/Exception.h"

namespace ems::modbus::master
{
	Connection::~Connection()
	{
		if (client_)
		{
			modbus_free(client_);
			client_ = nullptr;
		}

		if (bits_request_)
		{
			delete[] bits_request_;
			bits_request_ = nullptr;
		}

		if (registers_request_)
		{
			delete[] registers_request_;
			registers_request_ = nullptr;
		}
	}

	void Connection::set_timeout(const Timeout& timeout) const
	{
		modbus_set_response_timeout(client_, timeout.sec, timeout.usec);
	}

	void Connection::set_slave(int slave_id) const
	{
		modbus_set_slave(client_, slave_id);
	}

	void Connection::open() const
	{
#ifdef DEBUG
		modbus_set_debug(client_, true);
#endif

		if (modbus_connect(client_) < 0)
			throw ConnectionEstablishingException(modbus_strerror(errno));
	}

	void Connection::close() const
	{
		modbus_close(client_);
	}

	// Exception Connection::handle_exception(const int errno)
	// {
	// 	switch (errno)
	// 	{
	// 	case ENOTSOCK:
	// 		return static_cast<Exception>(NonSocketException(modbus_strerror(errno)));
	// 	default:
	// 		return static_cast<Exception>(CommunicationException(modbus_strerror(errno)));
	// 	}
	// }

	std::vector<uint8_t> Connection::read_bits(bits_reader reader, uint16_t addr, int quantity) const
	{
		memset(bits_request_, 0, sizeof(uint8_t) * REQUEST_LENGTH);
		static int packet_size = 0;
		if ((packet_size = reader(client_, addr, quantity, bits_request_)) < 0)
			throw CommunicationException(modbus_strerror(errno));

		std::vector<uint8_t> ret(packet_size);
		for (auto i = 0; i < packet_size; ++i)
			ret[i] = bits_request_[i];

		return ret;
	}

	std::vector<uint16_t> Connection::read_registers(registers_reader reader, uint16_t addr, int quantity) const
	{
		memset(registers_request_, 0, sizeof(uint16_t) * REQUEST_LENGTH);
		static int packet_size = 0;
		if ((packet_size = reader(client_, addr, quantity, registers_request_)) < 0)
			throw CommunicationException(modbus_strerror(errno));

		std::vector<uint16_t> ret(packet_size);
		for (auto i = 0; i < packet_size; ++i)
			ret[i] = registers_request_[i];

		return ret;
	}

	std::vector<uint8_t> Connection::read_coils(uint16_t addr, int quantity) const
	{
		return read_bits(modbus_read_bits, addr, quantity);
	}

	void Connection::write_coils(uint16_t addr, uint8_t value) const
	{
		if (modbus_write_bit(client_, addr, value) < 0)
			throw CommunicationException(modbus_strerror(errno));
	}

	void Connection::write_coils(uint16_t addr, const std::vector<uint8_t>& values) const
	{
		memset(bits_request_, 0, sizeof(uint8_t) * REQUEST_LENGTH);
		auto size = static_cast<int>(values.size());
		for (auto i = 0; i < size; ++i)
			bits_request_[i] = values[i];

		if (modbus_write_bits(client_, addr, size, bits_request_) < 0)
			throw CommunicationException(modbus_strerror(errno));
	}

	std::vector<uint8_t> Connection::read_discrete_inputs(uint16_t addr, int quantity) const
	{
		return read_bits(modbus_read_input_bits, addr, quantity);
	}

	std::vector<uint16_t> Connection::read_holding_registers(uint16_t addr, int quantity) const
	{
		return read_registers(modbus_read_registers, addr, quantity);
	}

	void Connection::write_holding_registers(uint16_t addr, uint16_t value) const
	{
		if (modbus_write_register(client_, addr, value) < 0)
			throw CommunicationException(modbus_strerror(errno));
	}

	void Connection::write_holding_registers(uint16_t addr, const std::vector<uint16_t>& values) const
	{
		memset(registers_request_, 0, sizeof(uint16_t) * REQUEST_LENGTH);
		auto size = static_cast<int>(values.size());
		for (auto i = 0; i < size; ++i)
			registers_request_[i] = values[i];

		if (modbus_write_registers(client_, addr, size, registers_request_) < 0)
			throw CommunicationException(modbus_strerror(errno));
	}

	std::vector<uint16_t> Connection::read_input_registers(uint16_t addr, int quantity) const
	{
		return read_registers(modbus_read_input_registers, addr, quantity);
	}
}
