//
// Created by eduar on 03/02/2021.
//

#include "alphavantage_data_feed.h"
#include <chrono>
#include <cpr/cpr.h>
#include <filesystem>
#include <iomanip>
#include <sstream>
namespace portfolio {

    alphavantage_data_feed::alphavantage_data_feed(
        const std::string_view &apiKey)
        : api_key(apiKey) {
        std::string path = "./";
        bool data_folder_exists = false;
        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            std::string filename(entry.path().filename().string());
            if (filename == "stock_data") {
                data_folder_exists = true;
                break;
            }
        }
        if (!data_folder_exists) {
            std::filesystem::create_directory("./stock_data");
        }
    }
    std::string alphavantage_data_feed::get_url(std::string_view asset_code,
                                                portfolio::timeframe tf) {
        std::string url = "https://www.alphavantage.co/query?function=";
        switch (tf) {
        case timeframe::minutes_15:
            return "15min";
        case timeframe::daily:
            url += "TIME_SERIES_DAILY";
            break;
        case timeframe::hourly:
            return "60min";
        case timeframe::monthly:
            url += "TIME_SERIES_MONTHLY";
            break;
        case timeframe::weekly:
            url += "TIME_SERIES_WEEKLY";
            break;
        }
        url += "&symbol=";
        url += asset_code;
        url += ".SAO&outputsize=full&apikey=";
        url += api_key;
        return url;
    }
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    std::string
    alphavantage_data_feed::minute_point_to_string(minute_point mp) {
        std::time_t tt = std::chrono::system_clock::to_time_t(mp);
        // std::tm tm = *std::localtime(&tt);
        std::tm tm = *std::gmtime(&tt);
        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d_%H-%M");
        return ss.str();
    }
#pragma clang diagnostic pop
    std::string alphavantage_data_feed::set_filename(
        std::string_view asset_code, std::string_view start_period,
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
    std::string
    alphavantage_data_feed::get_start_filename(std::string_view asset_code,
                                               timeframe tf) {
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
    minute_point
    alphavantage_data_feed::string_to_minute_point(std::string &str_mp) {
        std::tm tm = {};
        std::stringstream ss(str_mp);
        ss >> std::get_time(&tm, "%Y-%m-%d_%H-%M");
        std::chrono::system_clock::time_point tp =
            std::chrono::system_clock::from_time_t(mktime(&tm));
        // using namespace date;
        // std::cout << "str: " << str_mp << " - tp: " << tp << std::endl;
        using namespace std::chrono_literals;
        return std::chrono::floor<std::chrono::minutes>(tp);
    }

    data_feed_result alphavantage_data_feed::fetch(std::string_view asset_code,
                                                   minute_point start_period,
                                                   minute_point end_period,
                                                   timeframe tf) {
        bool has_previous_data = false;
        bool need_online_search = true;
        std::string path = "./stock_data";
        std::string file_path;
        std::string filename;
        price_map price_from_file;

        for (const auto &entry : std::filesystem::directory_iterator(path)) {
            filename = entry.path().filename().string();
            if (filename.starts_with(get_start_filename(asset_code, tf))) {
                file_path = entry.path().string();
                has_previous_data = true;
                break;
            }
        }

        if (has_previous_data) {
            std::string text = filename;
            std::replace(text.begin(), text.end(), '.', ' ');
            std::istringstream iss(text);
            std::vector<std::string> results(
                std::istream_iterator<std::string>{iss},
                std::istream_iterator<std::string>());
            minute_point file_start_point = string_to_minute_point(results[2]);
            minute_point file_end_point = string_to_minute_point(results[3]);
            if (file_start_point <= start_period &&
                file_end_point >= end_period) {
                need_online_search = false;
            } else {
                std::filesystem::remove(file_path);
            }
        }
        if (need_online_search) {
            return data_feed_result(get_data_from_alphavantage(
                asset_code, start_period, end_period, tf));
        } else {

            std::string line;
            std::ifstream fin(file_path);
            if (fin.is_open()) {
                std::getline(fin, line);
                fin.close();
            }
            nlohmann::json json_from_file = nlohmann::json::parse(line);

            for (auto &el : json_from_file.items()) {
                // std::cout << "FROM: " << el.key() << " : " << el.value() <<
                // "\n";
                std::string value = el.value();
                price_from_file[string_to_interval_points(el.key())] =
                    string_to_ohlc_prices(value);
            }

            price_map historical;
            for (auto &item : price_from_file) {
                if (item.first.first >= start_period &&
                    item.first.second <= end_period) {
                    historical[item.first] = item.second;
                }
            }

            return data_feed_result(historical);
        }
    }
    price_map alphavantage_data_feed::get_data_from_alphavantage(
        std::string_view asset_code, minute_point start_period,
        minute_point end_period, timeframe tf) {
        price_map historical_data;
        std::string url = get_url(asset_code, tf);
        cpr::Response r = cpr::Get(cpr::Url{url});
        if (r.status_code != 200) {
            throw std::runtime_error("Cannot request data: " + url);
        }
        nlohmann::json j_from_alphavantage = nlohmann::json::parse(r.text);
        if (j_from_alphavantage.contains("Error Message")) {
            throw std::runtime_error(
                "Request return error message: " +
                j_from_alphavantage.begin().value().get<std::string>() +
                " - URL: " + url + " - Verify if asset_code is valid.");
        }
        using namespace date::literals;
        using namespace std::chrono_literals;
        std::map<std::string, std::string> to_serialize;

        switch (tf) {
        case (timeframe::monthly):
            set_monthly_data(historical_data, to_serialize, start_period,
                             end_period, j_from_alphavantage);
            break;
        case (timeframe::weekly):
            set_weekly_data(historical_data, to_serialize, start_period,
                            end_period, j_from_alphavantage);
            break;
        case (timeframe::daily):
            set_daily_data(historical_data, to_serialize, start_period,
                           end_period, j_from_alphavantage);
            break;
        default:
            std::string str_tf;
            tf == timeframe::minutes_15 ? str_tf = "15 minutes"
                                        : str_tf = "Hourly";
            throw std::runtime_error(
                str_tf +
                " timeframe for B3 data is not supported by Alphavantage");
        }
        nlohmann::json j_to_serialize(to_serialize);
        std::string start_interval = to_serialize.begin()->first;
        std::string end_interval = to_serialize.rbegin()->first;
        std::replace(start_interval.begin(), start_interval.end(), '|', ' ');
        std::replace(end_interval.begin(), end_interval.end(), '|', ' ');
        std::istringstream iss_start(start_interval);
        std::istringstream iss_end(end_interval);
        std::vector<std::string> start_result(
            std::istream_iterator<std::string>{iss_start},
            std::istream_iterator<std::string>());
        std::vector<std::string> end_result(
            std::istream_iterator<std::string>{iss_end},
            std::istream_iterator<std::string>());
        std::string fp = "./stock_data/";
        fp += set_filename(asset_code, start_result[0], end_result[1], tf);
        std::ofstream fout(fp);
        fout << j_to_serialize.dump();
        fout.close();
        return historical_data;
    }
    void alphavantage_data_feed::set_daily_data(
        price_map &hist, std::map<std::string, std::string> &to_serialize,
        minute_point start_period, minute_point end_period,
        nlohmann::json j_data) {
        using namespace std::chrono_literals;
        for (auto &[key, value] : j_data["Time Series (Daily)"].items()) {
            std::string text = key;
            std::replace(text.begin(), text.end(), '-', ' ');
            std::istringstream iss(text);
            std::vector<std::string> results(
                std::istream_iterator<std::string>{iss},
                std::istream_iterator<std::string>());
            minute_point mp;
            mp = date::sys_days{date::year(std::stoi(results[0])) /
                                date::month(std::stoi(results[1])) /
                                date::day(std::stoi(results[2]))};
            std::string::size_type sz; // alias of size_t
            std::string st_open = value.at("1. open");
            std::string st_high = value.at("2. high");
            std::string st_low = value.at("3. low");
            std::string st_close = value.at("4. close");
            double open = std::stod(st_open, &sz);
            double high = std::stod(st_high, &sz);
            double low = std::stod(st_low, &sz);
            double close = std::stod(st_close, &sz);
            interval_points interval;
            ohlc_prices ohlc;
            if (mp >= start_period && mp <= end_period) {
                interval = std::make_pair(mp + 10h, mp + 18h);
                ohlc = std::make_tuple(open, high, low, close);
                hist[interval] = ohlc;
                to_serialize[interval_points_to_string(interval)] =
                    ohlc_prices_to_string(ohlc);
            } else {
                interval = std::make_pair(mp + 10h, mp + 18h);
                ohlc = std::make_tuple(open, high, low, close);
                to_serialize[interval_points_to_string(interval)] =
                    ohlc_prices_to_string(ohlc);
            }
        }
    }
    void alphavantage_data_feed::set_weekly_data(
        price_map &hist, std::map<std::string, std::string> &to_serialize,
        minute_point start_period, minute_point end_period,
        nlohmann::json j_data) {
        using namespace std::chrono_literals;
        interval_points interval;
        ohlc_prices ohlc;
        for (auto &[key, value] : j_data["Weekly Time Series"].items()) {
            std::string text = key;
            std::replace(text.begin(), text.end(), '-', ' ');
            std::istringstream iss(text);
            std::vector<std::string> results(
                std::istream_iterator<std::string>{iss},
                std::istream_iterator<std::string>());
            minute_point mp;
            mp = date::sys_days{date::year(std::stoi(results[0])) /
                                date::month(std::stoi(results[1])) /
                                date::day(std::stoi(results[2]))};
            std::string::size_type sz; // alias of size_t
            std::string st_open = value.at("1. open");
            std::string st_high = value.at("2. high");
            std::string st_low = value.at("3. low");
            std::string st_close = value.at("4. close");
            double open = std::stod(st_open, &sz);
            double high = std::stod(st_high, &sz);
            double low = std::stod(st_low, &sz);
            double close = std::stod(st_close, &sz);
            std::chrono::hours increment_open;
            std::chrono::hours increment_close;
            date::weekday wd{std::chrono::time_point_cast<date::days>(mp)};
            switch (wd.c_encoding()) {
            case (date::Friday.c_encoding()):
                increment_open = -86h;
                increment_close = 18h;
                break;
            case (date::Thursday.c_encoding()):
                increment_open = -62h;
                increment_close = 42h;
                break;
            case (date::Wednesday.c_encoding()):
                increment_open = -38h;
                increment_close = 66h;
                break;
            case (date::Tuesday.c_encoding()):
                increment_open = -14h;
                increment_close = 90h;
                break;
            case (date::Monday.c_encoding()):
                increment_open = 8h;
                increment_close = 114h;
            }
            interval =
                std::make_pair(mp + increment_open, mp + increment_close);
            ohlc = std::make_tuple(open, high, low, close);
            if (mp >= start_period && mp <= end_period) {
                hist[interval] = ohlc;
                to_serialize[interval_points_to_string(interval)] =
                    ohlc_prices_to_string(ohlc);
            } else {
                to_serialize[interval_points_to_string(interval)] =
                    ohlc_prices_to_string(ohlc);
            }
        }
    }
    void alphavantage_data_feed::set_monthly_data(
        price_map &hist, std::map<std::string, std::string> &to_serialize,
        minute_point start_period, minute_point end_period,
        nlohmann::json j_data) {
        using namespace std::chrono_literals;
        interval_points interval;
        ohlc_prices ohlc;
        for (auto &[key, value] : j_data["Monthly Time Series"].items()) {
            std::string text = key;
            std::replace(text.begin(), text.end(), '-', ' ');
            std::istringstream iss(text);
            std::vector<std::string> results(
                std::istream_iterator<std::string>{iss},
                std::istream_iterator<std::string>());
            minute_point mp_start, mp_end;
            mp_start = date::sys_days{date::year(std::stoi(results[0])) /
                                      date::month(std::stoi(results[1])) /
                                      date::day(1)};
            mp_end = date::sys_days{date::year(std::stoi(results[0])) /
                                    date::month(std::stoi(results[1])) /
                                    date::day(std::stoi(results[2]))};
            std::string::size_type sz; // alias of size_t
            std::string st_open = value.at("1. open");
            std::string st_high = value.at("2. high");
            std::string st_low = value.at("3. low");
            std::string st_close = value.at("4. close");
            double open = std::stod(st_open, &sz);
            double high = std::stod(st_high, &sz);
            double low = std::stod(st_low, &sz);
            double close = std::stod(st_close, &sz);
            std::chrono::hours increment_open;
            date::weekday wd{
                std::chrono::time_point_cast<date::days>(mp_start)};
            switch (wd.c_encoding()) {
            case (date::Saturday.c_encoding()):
                increment_open = 58h;
                break;
            case (date::Monday.c_encoding()):
                increment_open = 34h;
                break;
            default:
                increment_open = 10h;
            }
            interval = std::make_pair(mp_start + increment_open, mp_end + 18h);
            ohlc = std::make_tuple(open, high, low, close);
            if (mp_end >= start_period && mp_end <= end_period) {
                hist[interval] = ohlc;
                to_serialize[interval_points_to_string(interval)] =
                    ohlc_prices_to_string(ohlc);
            } else {
                to_serialize[interval_points_to_string(interval)] =
                    ohlc_prices_to_string(ohlc);
            }
        }
    }
    std::string alphavantage_data_feed::interval_points_to_string(
        interval_points interval) {
        std::string str_start = minute_point_to_string(interval.first);
        std::string str_end = minute_point_to_string(interval.second);
        return str_start + "|" + str_end;
    }
    std::string
    alphavantage_data_feed::ohlc_prices_to_string(ohlc_prices &ohlc) {
        std::string open_str = std::to_string(std::get<0>(ohlc));
        std::string high_str = std::to_string(std::get<1>(ohlc));
        std::string low_str = std::to_string(std::get<2>(ohlc));
        std::string close_str = std::to_string(std::get<3>(ohlc));
        return open_str + " " + high_str + " " + low_str + " " + close_str;
    }
    interval_points alphavantage_data_feed::string_to_interval_points(
        std::string str_interval) {
        std::replace(str_interval.begin(), str_interval.end(), '|', ' ');

        std::istringstream iss(str_interval);

        std::vector<std::string> result(std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>());

        minute_point start = string_to_minute_point(result[0]);
        minute_point end = string_to_minute_point(result[1]);

        return std::make_pair(start, end);
    }
    ohlc_prices
    alphavantage_data_feed::string_to_ohlc_prices(std::string &str_ohlc) {
        std::istringstream iss(str_ohlc);

        std::vector<std::string> result(std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>());
        std::string::size_type sz; // alias of size_t
        double open = std::stod(result[0], &sz);
        double high = std::stod(result[1], &sz);
        double low = std::stod(result[2], &sz);
        double close = std::stod(result[3], &sz);
        return std::make_tuple(open, high, low, close);
    }

} // namespace portfolio