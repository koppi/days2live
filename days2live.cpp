#include <chrono>
#include <iostream>
#include <iomanip>
#include <format>
#include <thread>
#include <cmath>
#include <string>
#include <cstdlib>
#include <sstream>

using namespace std;
namespace chrono = std::chrono;

void msleep(unsigned long ms) {
    std::this_thread::sleep_for(chrono::milliseconds(ms));
}

int do_stats(const chrono::sys_days& birthday, int years_to_life);

// Helper: Parse "YYYY-MM-DD" to chrono::sys_days, returns true on success
bool parse_bday(const std::string& bday_str, chrono::sys_days& out) {
    int y, m, d;
    char dash1, dash2;
    std::istringstream iss(bday_str);
    if (!(iss >> y >> dash1 >> m >> dash2 >> d)) return false;
    if (dash1 != '-' || dash2 != '-') return false;
    try {
        chrono::year_month_day ymd{
            chrono::year{y},
            chrono::month{static_cast<unsigned>(m)},
            chrono::day{static_cast<unsigned>(d)}
        };
        if (!ymd.ok()) return false;
        out = chrono::sys_days{ymd};
        return true;
    } catch (...) {
        return false;
    }
}

void print_help(const char* prog) {
    std::cout <<
        "Usage: " << prog << " [years_to_life] [YYYY-MM-DD]\n"
        "  years_to_life   Optional. Average lifetime in years (default: 125)\n"
        "  YYYY-MM-DD      Optional. Birthday in format YYYY-MM-DD (default: 1978-10-09)\n"
        "  -h, --help      Show this help and exit\n"
        "Examples:\n"
        "  " << prog << "\n"
        "  " << prog << " 100\n"
        "  " << prog << " 90 2000-01-01\n";
}

int main(int argc, char* argv[]) {
    // Help option
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            print_help(argv[0]);
            return 0;
        }
    }

    // Defaults
    int years_to_life = 125;
    chrono::year_month_day default_bday{chrono::year{1978}, chrono::month{10}, chrono::day{9}};
    chrono::sys_days birthday = chrono::sys_days{default_bday};

    // Argument parsing
    // argv[1] = years_to_life (optional)
    // argv[2] = bday (optional, format YYYY-MM-DD)
    if (argc > 1) {
        try {
            years_to_life = std::stoi(argv[1]);
        } catch (...) {
            std::cerr << "Invalid value for years_to_life, using default: " << years_to_life << std::endl;
        }
    }
    if (argc > 2) {
        if (!parse_bday(argv[2], birthday)) {
            std::cerr << "Invalid birthday format (use YYYY-MM-DD), using default: 1978-10-09" << std::endl;
        }
    }

    while (true) {
        do_stats(birthday, years_to_life);
        msleep(20);
        cout << "\033[2J\033[1;1H";
    }
}

int do_stats(const chrono::sys_days& birthday, int years_to_life) {
    using chrono::year;
    using chrono::month;
    using chrono::day;
    using chrono::year_month_day;
    using chrono::sys_days;
    using chrono::zoned_time;
    using chrono::current_zone;
    using chrono::years;
    using chrono::months;
    using chrono::days;
    using chrono::hours;
    using chrono::minutes;
    using chrono::seconds;
    using chrono::milliseconds;
    using chrono::microseconds;
    using chrono::nanoseconds;

    // Death date (years_to_life years after birthday): do arithmetic on year_month_day
    auto death_ymd = year_month_day{birthday} + years{years_to_life};
    sys_days death = sys_days{death_ymd};

    // Get current system time and local time in the default time zone
    auto now = chrono::system_clock::now();
    sys_days today = chrono::floor<days>(now);

    // Use the current time zone (from the OS or TZ env variable)
    auto zone = current_zone();
    auto zt = zoned_time{zone, now};
    auto birthday_zt = zoned_time{zone, birthday};
    auto death_zt = zoned_time{zone, death};

    // Print system and local time
    std::cout << " System time: " << std::format("{:%A %F %T %Z}", now) << std::endl;
    std::cout << "  Local time: " << std::format("{:%A %F %T %Z}", zt) << std::endl;

    std::cout << "\n Average lifetime (as of 2025): " << years_to_life << " years\n";
    std::cout << "         Your Birthday: " << std::setw(8) << std::format("{:%A}", birthday) << " " << std::format("{:%F %T %Z}", birthday_zt) << std::endl;
    std::cout << "         Your Deathday: " << std::setw(8) << std::format("{:%A}", death) << " " << std::format("{:%F %T %Z}", death_zt) << std::endl;

    auto time_left = death - now;
    auto time_left_s = chrono::duration_cast<seconds>(death - now).count();
    std::cout << "\n You are " << (time_left_s > 0 ? "alive! Time to life left:" : "dead! Time since death:") << "\n\n";

    // Use floating point durations for precision, with 6 digits
    auto lifetime_y      = std::chrono::duration<double, std::ratio<31556952>>(death - now).count();     // mean solar year
    auto lifetime_month  = std::chrono::duration<double, std::ratio<2629746>>(death - now).count();      // mean month
    auto lifetime_d      = std::chrono::duration<double, std::ratio<86400>>(death - now).count();
    auto lifetime_h      = std::chrono::duration<double, std::ratio<3600>>(death - now).count();
    auto lifetime_min    = std::chrono::duration<double, std::ratio<60>>(death - now).count();
    auto lifetime_s      = std::chrono::duration<double>(death - now).count();
    auto lifetime_ms     = std::chrono::duration<double, std::milli>(death - now).count();
    auto lifetime_us     = std::chrono::duration<double, std::micro>(death - now).count();
    auto lifetime_ns     = std::chrono::duration<double, std::nano>(death - now).count();

    constexpr int pad = 48;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << std::setfill(' ') << std::setw(pad) << std::abs(lifetime_y) << "        years"  << std::endl;
    std::cout << std::setfill(' ') << std::setw(pad) << std::abs(lifetime_month) << "       months" << std::endl;
    std::cout << std::setfill(' ') << std::setw(pad) << std::abs(lifetime_d) << "         days"  << std::endl;
    std::cout << std::setfill(' ') << std::setw(pad) << std::abs(lifetime_h) << "        hours" << std::endl;
    std::cout << std::setfill(' ') << std::setw(pad) << std::abs(lifetime_min) << "      minutes" << std::endl;
    std::cout << std::setfill(' ') << std::setw(pad) << std::abs(lifetime_s) << "      seconds" << std::endl;
    std::cout << std::setfill(' ') << std::setw(pad) << std::abs(lifetime_ms) << " milliseconds" << std::endl;
    std::cout << std::setfill(' ') << std::setw(pad) << std::abs(lifetime_us) << " microseconds" << std::endl;
    std::cout << std::setfill(' ') << std::setw(pad) << std::abs(lifetime_ns) << "  nanoseconds" << std::endl;

    return 0;
}