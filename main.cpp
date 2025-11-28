#include <iostream>
#include <ostream>
#include <bits/this_thread_sleep.h>
#include "connections/RtuConnection.h"
#include <modbus.h>
#include <thread>

using namespace ems::modbus;
using namespace ems::modbus::master;

int main() {
    const char* DEVICE = "/dev/com4";

    modbus_t* ctx = modbus_new_rtu(DEVICE, 9600, 'N', 8, 1);
    if (ctx == nullptr) {
        std::cerr << "Unable to create libmodbus context\n";
        return -1;
    }


    modbus_set_slave(ctx, 1);

    // 打开串口
    if (modbus_connect(ctx) == -1) {
        std::cerr << "Connection failed: " << modbus_strerror(errno) << "\n";
        modbus_free(ctx);
        return -1;
    }

    std::cout << "Modbus RTU Connected!" << std::endl;

    // 设置超时
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    modbus_set_response_timeout(ctx, timeout.tv_sec, timeout.tv_usec);

    while (true) {
        uint16_t tab_reg[10] = {0};

        int rc = modbus_read_registers(ctx,
                                       1,     // 起始地址 = 0x0001
                                       10,    // 数量
                                       tab_reg);

        if (rc == -1) {
            std::cerr << "Read error: " << modbus_strerror(errno) << std::endl;
        } else {
            std::cout << "Read success: ";
            for (int i = 0; i < 10; ++i) {
                std::cout << tab_reg[i] << " ";
            }
            std::cout << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}