#include <iostream>
#include <ostream>
#include <bits/this_thread_sleep.h>

#include "connections/RtuConnection.h"
#include "common/Config.h"

using namespace ems::modbus;
using namespace ems::modbus::master;

int main() {
    RtuConfig config;

    config.serial_port = "/dev/com4";
    config.baud_rate = 9600;
    config.data_bits = 8;
    config.stop_bits = 1;
    config.parity = 'N';

    RtuConnection rtu(config);

    Timeout timeout;
    timeout.sec = 3;
    timeout.usec = 0;
    rtu.set_timeout(timeout);

    rtu.set_slave(1);

    rtu.enable_debug();
    rtu.open();
    std::cout << "RTU Connected" << std::endl;

    while (true) {
        auto regs = rtu.read_holding_registers(0x0001, 10);
        std::cout << "Read Holding registers: " << std::endl;

        for (size_t i = 0; i < regs.size(); i++) {
            std::cout << regs[i] << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}