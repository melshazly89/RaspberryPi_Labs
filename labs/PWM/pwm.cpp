#include <stdio.h>
#include <string>
#include <unistd.h>
#include <thread>
#include <chrono>

class sysfs_pwm {
public:
    std::string pwm_chip;
    std::string pwm_channel;
    std::string pwm_path;
    sysfs_pwm(const std::string& pwm_chip, const std::string& pwm_channel) {
        this->pwm_chip = pwm_chip;
        this->pwm_channel = pwm_channel;
        this->pwm_path = "/sys/class/pwm/" + pwm_chip + "/pwm" + pwm_channel;
    }
    void export_pwm() {
        std::string export_path = "/sys/class/pwm/" + pwm_chip + "/export";
        FILE* fp = fopen(export_path.c_str(), "w");
        if (fp == nullptr) {
            perror("Failed to open PWM export");
            return;
        }
        fprintf(fp, "%s", pwm_channel.c_str());
        fclose(fp);
        sleep(1); // Wait for the system to create the PWM folder
    }
    void unexport_pwm() {
        std::string unexport_path = "/sys/class/pwm/" + pwm_chip + "/unexport";
        FILE* fp = fopen(unexport_path.c_str(), "w");
        if (fp == nullptr) {
            perror("Failed to open PWM unexport");
            return;
        }
        fprintf(fp, "%s", pwm_channel.c_str());
        fclose(fp);
        sleep(1); // Wait for the system to remove the PWM folder
    }
    void set_pwm_period(int period_ns) {
        std::string period_path = pwm_path + "/period";
        FILE* fp = fopen(period_path.c_str(), "w");
        if (fp == nullptr) {
            perror("Failed to open PWM period");
            return;
        }
        fprintf(fp, "%d", period_ns);
        fclose(fp);
        sleep(1); // Wait for the system to apply the period change
    }
    void set_pwm_duty_cycle(int duty_cycle_ns) {
        std::string duty_cycle_path = pwm_path + "/duty_cycle";
        FILE* fp = fopen(duty_cycle_path.c_str(), "w");
        if (fp == nullptr) {
            perror("Failed to open PWM duty cycle");
            return;
        }
        fprintf(fp, "%d", duty_cycle_ns);
        fclose(fp);
        sleep(1); // Wait for the system to apply the duty cycle change
    }
    void enable_pwm() {
        std::string enable_path = pwm_path + "/enable";
        FILE* fp = fopen(enable_path.c_str(), "w");
        if (fp == nullptr) {
            perror("Failed to open PWM enable");
            return;
        }
        fprintf(fp, "1");
        fclose(fp);
        sleep(1); // Wait for the system to apply the duty cycle change
    }
    void disable_pwm() {
        std::string enable_path = pwm_path + "/enable";
        FILE* fp = fopen(enable_path.c_str(), "w");
        if (fp == nullptr) {
            perror("Failed to open PWM enable");
            return;
        }
        fprintf(fp, "0");
        fclose(fp);
        sleep(1); // Wait for the system to apply the duty cycle change
    }
};
int main() {
    sysfs_pwm pwm("pwmchip0", "0");
    const u_int64_t period = 1000000; // 1 ms
    const u_int64_t duty_cycle = 500000; // 0.5 ms
    pwm.export_pwm();
    pwm.set_pwm_period(period); // 10 ms
    pwm.set_pwm_duty_cycle(duty_cycle); // 0.5 ms
    pwm.enable_pwm();
    
    std::this_thread::sleep_for(std::chrono::seconds(5)); // Wait for 5 second
    
    for (int i = 0; i < 100; i += 5) {
        pwm.set_pwm_duty_cycle(period * i / 100); // Increase duty cycle by 0.5 ms every second
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Wait for 50 milli second
    }
    
    for (int i = 100; i >= 0; i -= 5) {
        pwm.set_pwm_duty_cycle(period * i / 100); // Decrease duty cycle by 0.5 ms every second
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Wait for 50 milli second
    }
    
    
    pwm.disable_pwm();
    pwm.unexport_pwm();
    return 0;
}