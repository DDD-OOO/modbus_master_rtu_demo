#include <iostream>
#include <ostream>
#include <bits/this_thread_sleep.h>
#include <modbus.h>
#include <thread>

#include <chrono>
#include <iomanip>
#include <sstream>

void print_now(const std::string& prefix = "")
{
    using namespace std::chrono;

    auto now = system_clock::now();
    auto now_time = system_clock::to_time_t(now);

    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::tm tm_buf{};
#ifdef _WIN32
    localtime_s(&tm_buf, &now_time);
#else
    localtime_r(&now_time, &tm_buf);
#endif

    std::stringstream ss;
    ss << std::put_time(&tm_buf, "%H:%M:%S")
       << "." << std::setw(3) << std::setfill('0') << ms.count();

    if (!prefix.empty())
        std::cout << prefix;

    std::cout << ss.str() << std::endl;
}

int main() {
    const char* DEVICE = "/dev/com2";

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

        auto start = std::chrono::steady_clock::now();
        int rc = modbus_read_registers(ctx, 1, 50, tab_reg);
        auto end = std::chrono::steady_clock::now();

        auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "modbus_read_registers cost = " << cost << " ms\n";


        if (rc == -1) {
            std::cerr << "Read error: " << modbus_strerror(errno) << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}