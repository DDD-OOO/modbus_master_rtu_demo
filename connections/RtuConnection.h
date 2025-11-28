//
// Created by Eric Li on 25-5-24.
//

/*
 * @brief RTU连接类
 *
 * @author Eric Li
 * @date 2025-05-24
 */
#ifndef EMS_MODBUS_MASTER_RTU_CONNECTION_H
#define EMS_MODBUS_MASTER_RTU_CONNECTION_H

#include "Connection.h"

namespace ems::modbus::master
{
    class RtuConnection final : public Connection
    {
    public:
        explicit RtuConnection(const RtuConfig& config)
            : Connection(config)
        {
            client_ = modbus_new_rtu(
                config.serial_port.c_str(),
                config.baud_rate,
                config.parity,
                config.data_bits,
                config.stop_bits
                );
        }
    };
}

#endif //EMS_MODBUS_MASTER_RTU_CONNECTION_H
