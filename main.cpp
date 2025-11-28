#include <iostream>
#include <ostream>
#include <bits/this_thread_sleep.h>
#include <modbus.h>
#include <thread>

int main() {
    const char* DEVICE = "/dev/ttyUSB0";

    modbus_t* ctx = modbus_new_rtu(DEVICE, 9600, 'N', 8, 1);
    if (ctx == nullptr) {
        return -1;
    }

    modbus_set_slave(ctx, 1);

    modbus_set_debug(ctx, true);

    if (modbus_connect(ctx) == -1) {
        std::cerr << "Connection failed: " << modbus_strerror(errno) << "\n";
        modbus_free(ctx);
        return -1;
    }

    std::cout << "Modbus RTU Connected!" << std::endl;


    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 500;
    modbus_set_response_timeout(ctx, timeout.tv_sec, timeout.tv_usec);

    while (true) {
        uint16_t tab_reg[10] = {0};

        int rc = modbus_read_registers(ctx, 1, 10, tab_reg);

        if (rc == -1) {
            std::cerr << "Read error: " << modbus_strerror(errno) << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}