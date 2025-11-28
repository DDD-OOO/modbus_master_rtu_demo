//
// Created by Eric Li on 25-5-23.
//

/*
 * @brief modbus主站连接客户端
 *
 * @author Eric Li
 * @date 2025-05-23
 */
#ifndef EMS_MODBUS_MASTER_CONNECTION_H
#define EMS_MODBUS_MASTER_CONNECTION_H

#include <vector>

#include <modbus.h>

#include "common/Config.h"

namespace ems::modbus::master
{
	class Connection
	{
	public:
		/*
		* @brief 析构函数需要回收内存
		*/
		virtual ~Connection();

		/*
		 * @brief 工厂模式实现对应通信协议的实例化
		 */
		template <ConnectionConfig T>
		explicit Connection(const T& config)
			: client_(nullptr)
			, bits_request_(nullptr)
			, registers_request_(nullptr)
		{
			bits_request_      = new uint8_t[REQUEST_LENGTH];
			registers_request_ = new uint16_t[REQUEST_LENGTH];
		}

		/*
		* @brief 设置回复超时
		*
		* @param Linux类型的timeout
		*/
		void set_timeout(const Timeout& timeout) const;

		/*
		 * @brief 设置从机id
		 */
		void set_slave(int slave_id) const;

		/*
		* @brief 关闭连接
		*/
		void close() const;

		/*
		* @brief 打开连接
		*
		* @param slave_id 连接的从站id
		*
		* @exception ConnectionEstablishingException 建立连接错误
		*/
		void open() const;

		void enable_debug() const {
			modbus_set_debug(client_, TRUE);
		}

		/*
		* @brief 读取线圈值
		*
		* @param addr 起始地址
		* @param quantity 线圈数量
		*
		* @return
		*
		* @exception CommunicationException 通信异常
		*/
		[[nodiscard]] std::vector<uint8_t> read_coils(uint16_t addr, int quantity) const;

		/*
		 * @brief 写入单个线圈
		 *
		 * @param addr 起始地址
		 * @param value 写入的值
		 *
		* @exception CommunicationException 通信异常
		 */
		void write_coils(uint16_t addr, uint8_t value) const;

		/*
		* @brief 写入线圈
		*
		* @param addr 起始地址
		* @param values 连续写入的值
		*
		* @exception CommunicationException 通信异常
		*/
		void write_coils(uint16_t addr, const std::vector<uint8_t>& values) const;

		/*
		* @brief 读取离散寄存器值
		*
		* @param addr 起始地址
		* @param 寄存器数量
		*
		* @return
		*
		* @exception CommunicationException 通信异常
		*/
		[[nodiscard]] std::vector<uint8_t> read_discrete_inputs(uint16_t addr, int quantity) const;

		/*
		 * @brief 读取保持寄存器
		 *
		 * @param addr 起始地址
		 * @param quantity 寄存器数量
		 *
		 * @return std::vector<uint16_t>
		 *
		 * @exception CommunicationException 通信异常
		 */
		[[nodiscard]] std::vector<uint16_t> read_holding_registers(uint16_t addr, int quantity) const;

		/*
		 * @brief 写入保持寄存器
		 *
		 * @param addr 起始地址
		 * @param value 写入数据
		 *
		 * @exception CommunicationException 通信异常
		 */
		void write_holding_registers(uint16_t addr, uint16_t value) const;

		/*
		 * @brief 写入保持寄存器
		 *
		 * @param addr 起始地址
		 * @param values 连续数据
		 *
		 * @exception CommunicationException 通信异常
		 */
		void write_holding_registers(uint16_t addr, const std::vector<uint16_t>& values) const;

		/*
		 * @brief 读取输入寄存器
		 *
		 * @param addr 起始地址
		 * @param quantity 寄存器数量
		 *
		 * @exception CommunicationException 通信异常
		 */
		[[nodiscard]] std::vector<uint16_t> read_input_registers(uint16_t addr, int quantity) const;

	protected:
		/*
		* @brief modbus连接客户端
		*/
		modbus_t* client_;

	private:
		/*
		 * @brief 定义读取线圈和离散寄存器的函数
		 */
		using bits_reader = int(*)(modbus_t*, int, int, uint8_t*);

		/*
		 * @brief 读取线圈和离散寄存器
		 *
		 * @param reader libmodbus调用的函数
		 * @param addr 起始地址
		 * @param quantity 读取数量
		 *
		 * @return std::vector<bool> 布尔值
		 *
		 * @exception CommunicationException 连接异常
		 */
		std::vector<uint8_t> read_bits(bits_reader reader, uint16_t addr, int quantity) const;

		/*
		 * @brief 定义读取保持寄存器和输入寄存器的函数
		 */
		using registers_reader = int(*)(modbus_t*, int, int, uint16_t*);

		/*
		 * @brief 读取保持寄存器和输入寄存器
		 *
		 * @param reader libmodbus调用的函数
		 * @param addr 起始地址
		 * @param quantity 读取数量
		 *
		 * @return std::vector<uint16_t>
		 *
		 * @exception CommunicationException 连接异常
		 */
		std::vector<uint16_t> read_registers(registers_reader reader, uint16_t addr, int quantity) const;

		// static Exception handle_exception(const int errno) ;

		/*
		 * @brief 读写数据长度
		 */
		static constexpr int REQUEST_LENGTH = 256;

		/*
		 * @brief 读写线圈和离散寄存器用
		 */
		uint8_t* bits_request_;

		/*
		 * @brief 读写保持寄存器和输入寄存器用
		 */
		uint16_t* registers_request_;
	};
}

#endif //EMS_MODBUS_MASTER_CONNECTION_H
