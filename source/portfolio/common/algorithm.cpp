//
// Created by eduardo on 15/03/2021.
//

#include "algorithm.h"
#include <iostream>
#include <regex>
#include <stdexcept>
#include <vector>

namespace portfolio {

    std::string minute_point_to_string(minute_point mp) {
        return date::format("%Y-%m-%d_%H-%M", mp);
    }

    std::string interval_points_to_string(interval_points interval) {
        std::string str_start = minute_point_to_string(interval.first);
        std::string str_end = minute_point_to_string(interval.second);
        return str_start + "|" + str_end;
    }
    int parse_int(const char *b, const char *e) {
        int i = 0;
        for (; b != e; ++b)
            i = 10 * i + (*b - '0');
        return i;
    }
    minute_point string_to_minute_point(std::string_view str_mp) {

        // YYYY - MM - DD _ HH - MM
        // 0123 4 56 7 89 0 12 3 45

        std::string str(str_mp);
        int y = parse_int(str.data() + 0, str.data() + 4);
        int m = parse_int(str.data() + 5, str.data() + 7);
        int d = parse_int(str.data() + 8, str.data() + 10);
        int h = parse_int(str.data() + 11, str.data() + 13);
        int M = parse_int(str.data() + 14, str.data() + 16);
        // return sys_days{year{y}/m/d} + hours{h} + minutes{M} + seconds{0};

        //        std::chrono::system_clock::time_point dt;
        //        std::stringstream ss(str);
        //        ss >> date::parse("%Y-%m-%d_%H-%M", dt);

        // ss >> std::chrono::parse("%Y-%m-%d_%H-%M", dt);
        // return std::chrono::floor<std::chrono::minutes>(dt);
        return std::chrono::floor<std::chrono::minutes>(
            date::sys_days{date::year{y} / m / d} + std::chrono::hours{h} +
            std::chrono::minutes{M} + std::chrono::seconds{0});
    }

    interval_points string_to_interval_points(std::string_view str_interval) {
        std::string str(str_interval);
        std::replace(str.begin(), str.end(), '|', ' ');
        std::istringstream iss(str);
        std::vector<std::string> result(std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>());
        minute_point start = string_to_minute_point(result[0]);
        minute_point end = string_to_minute_point(result[1]);
        return std::make_pair(start, end);
    }
    std::string set_filename(std::string_view asset_code,
                             std::string_view start_period,
                             std::string_view end_period, timeframe tf) {
        std::string filename{asset_code};
        switch (tf) {
        case (timeframe::monthly):
            filename += ".MONTHLY";
            break;
        case (timeframe::weekly):
            filename += ".WEEKLY";
            break;
        case (timeframe::daily):
            filename += ".DAILY";
            break;
        case (timeframe::hourly):
            filename += ".HOURLY";
            break;
        case (timeframe::minutes_15):
            filename += "MINUTES15";
        }
        filename += ".";
        filename += std::string{start_period};
        filename += ".";
        filename += std::string{end_period};
        filename += ".data";
        return filename;
    }
    std::string start_filename(std::string_view asset_code, timeframe tf) {
        std::string start_filename{asset_code};
        switch (tf) {
        case (timeframe::monthly):
            start_filename += ".MONTHLY";
            break;
        case (timeframe::weekly):
            start_filename += ".WEEKLY";
            break;
        case (timeframe::daily):
            start_filename += ".DAILY";
            break;
        case (timeframe::hourly):
            start_filename += ".HOURLY";
            break;
        case (timeframe::minutes_15):
            start_filename += "MINUTES15";
        }
        return start_filename;
    }
    bool is_floating(std::string_view str_view) {
        std::string str(str_view);
        const unsigned int len = str.length();
        if (len == 0) {
            return false;
        } else if (len == 1) {
            if (std::isdigit(str[0])) {
                return true;
            } else {
                return false;
            }
        } else if (len == 2) {
            if ((std::isdigit(str[0]) || str[0] == '-' || str[0] == '+') &&
                std::isdigit(str[1])) {
                return true;
            } else {
                return false;
            }
        } else {
            if (!std::isdigit(str[len - 1])) {
                return false;
            } else {
                bool start_with_signal = str[0] == '-' || str[0] == '+';
                bool start_is_valid = std::isdigit(str[0]) || start_with_signal;
                if (!start_is_valid) {
                    return false;
                } else {
                    bool has_point = false;
                    for (int i = 1; i < len - 1; ++i) {
                        if (i == 1 && start_with_signal &&
                            !std::isdigit(str[1])) {
                            return false;
                        } else {
                            if (str[i] == '.') {
                                if (has_point) {
                                    return false;
                                } else {
                                    has_point = true;
                                }
                            } else if (!std::isdigit(str[i])) {
                                return false;
                            }
                        }
                    }
                    return true;
                }
            }
        }
    }
} // namespace portfolio