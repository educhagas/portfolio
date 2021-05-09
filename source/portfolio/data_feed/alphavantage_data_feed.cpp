//
// Created by eduardo on 03/02/2021.
//

#include "alphavantage_data_feed.h"
#include "portfolio/common/algorithm.h"
#include <chrono>
#include <cpr/cpr.h>
#include <filesystem>
#include <iostream>
namespace portfolio {

    alphavantage_data_feed::alphavantage_data_feed(
        const std::string_view &apiKey, bool api_key_is_free)
        : api_key_(apiKey), api_key_is_free_(api_key_is_free) {
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
    std::string
    alphavantage_data_feed::generate_url(std::string_view asset_code,
                                         portfolio::timeframe tf) {
        std::string url = "https://www.alphavantage.co/query?function=";
        switch (tf) {
        case timeframe::minutes_15:
            return "15min";
        case timeframe::daily:
            url += "TIME_SERIES_DAILY_ADJUSTED&outputsize=full";
            break;
        case timeframe::hourly:
            return "60min";
        case timeframe::monthly:
            url += "TIME_SERIES_MONTHLY_ADJUSTED";
            break;
        case timeframe::weekly:
            url += "TIME_SERIES_WEEKLY_ADJUSTED";
            break;
        }
        url += "&symbol=";
        url += asset_code;
        url += "&apikey=";
        url += api_key_;
        return url;
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
            if (filename.starts_with(start_filename(asset_code, tf))) {
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
            minute_point file_start_point;
            minute_point file_end_point;
            if (results.size() == 5) {
                file_start_point = string_to_minute_point(results[2]);
                file_end_point = string_to_minute_point(results[3]);
            } else {
                file_start_point = string_to_minute_point(results[3]);
                file_end_point = string_to_minute_point(results[4]);
            }

            if (file_start_point <= start_period &&
                file_end_point >= end_period) {
                need_online_search = false;
            } else {
                std::filesystem::remove(file_path);
            }
        }
        if (need_online_search) {
            price_map hist;
            if (!request_online_data(hist, asset_code, start_period, end_period,
                                     tf)) {
                std::cerr << "Error on data requesting." << std::endl;
            }
            return data_feed_result(hist);
        } else {
            std::string line;
            std::ifstream fin(file_path);
            if (fin.is_open()) {
                std::getline(fin, line);
                fin.close();
            }
            nlohmann::json json_from_file = nlohmann::json::parse(line);
            bool has_error = false;
            for (auto &el : json_from_file.items()) {
                std::string value = el.value();
                ohlc_prices ohlc;
                has_error = !ohlc.from_string(value);
                if (has_error) {
                    std::cerr << "Error on data reading." << std::endl;
                    break;
                }
                price_from_file[string_to_interval_points(el.key())] = ohlc;
            }
            price_map historical;
            if (!has_error) {
                for (auto &item : price_from_file) {
                    if (item.first.first >= start_period &&
                        item.first.second <= end_period) {
                        historical[item.first] = item.second;
                    }
                }
            }
            return data_feed_result(historical);
        }
    }
    bool alphavantage_data_feed::request_online_data(
        price_map &historical_data, std::string_view asset_code,
        minute_point start_period, minute_point end_period, timeframe tf) {
        // If the API key is free, wait 20 seconds to ensure that there will be
        // a maximum of 5 requests per minute.
        if (api_key_is_free_) {
            std::this_thread::sleep_for(std::chrono::seconds(20));
        }
        std::string url = generate_url(asset_code, tf);
        cpr::Response r = cpr::Get(cpr::Url{url});
        if (r.status_code != 200) {
            throw std::runtime_error("Cannot request data: " + url);
        }
        nlohmann::json j_from_alphavantage = nlohmann::json::parse(r.text);
        if (!j_from_alphavantage.contains("Meta Data")) {
            throw std::runtime_error(
                "Request return error message: " +
                j_from_alphavantage.begin().value().get<std::string>() +
                " - URL: " + url + " - Verify if asset_code is valid.");
        }
        using namespace date::literals;
        using namespace std::chrono_literals;
        std::map<std::string, std::string> to_serialize;
        bool has_error;
        switch (tf) {
        case (timeframe::monthly):
            has_error =
                !set_monthly_data(historical_data, to_serialize, start_period,
                                  end_period, j_from_alphavantage);
            break;
        case (timeframe::weekly):
            has_error =
                !set_weekly_data(historical_data, to_serialize, start_period,
                                 end_period, j_from_alphavantage);
            break;
        case (timeframe::daily):
            has_error =
                !set_daily_data(historical_data, to_serialize, start_period,
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
        if (has_error) {
            return false;
        } else {
            nlohmann::json j_to_serialize(to_serialize);
            std::string start_interval = to_serialize.begin()->first;
            std::string end_interval = to_serialize.rbegin()->first;
            std::replace(start_interval.begin(), start_interval.end(), '|',
                         ' ');
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
            return true;
        }
    }
    bool alphavantage_data_feed::set_daily_data(
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
            std::string st_open;
            std::string st_high;
            std::string st_low;
            std::string st_close;
            try {
                st_open = value["1. open"];
                st_high = value["2. high"];
                st_low = value["3. low"];
                st_close = value["4. close"];
            } catch (nlohmann::json::exception &e) {
                throw std::runtime_error("Fatal error: " +
                                         std::string(e.what()));
            }
            std::string::size_type sz; // alias of size_t
            double open, high, low, close;
            if (is_floating(st_open)) {
                open = std::stod(st_open, &sz);
            } else {
                return false;
            }
            if (is_floating(st_high)) {
                high = std::stod(st_high, &sz);
            } else {
                return false;
            }
            if (is_floating(st_low)) {
                low = std::stod(st_low, &sz);
            } else {
                return false;
            }
            if (is_floating(st_close)) {
                close = std::stod(st_close, &sz);
            } else {
                return false;
            }

            interval_points interval;
            ohlc_prices ohlc;
            if (mp >= start_period && mp <= end_period) {
                interval = std::make_pair(mp + 10h, mp + 18h);
                ohlc.set_prices(open, high, low, close);
                hist[interval] = ohlc;
                to_serialize[interval_points_to_string(interval)] =
                    ohlc.to_string();
            } else {
                interval = std::make_pair(mp + 10h, mp + 18h);
                ohlc.set_prices(open, high, low, close);
                to_serialize[interval_points_to_string(interval)] =
                    ohlc.to_string();
            }
        }
        return true;
    }
    bool alphavantage_data_feed::set_weekly_data(
        price_map &hist, std::map<std::string, std::string> &to_serialize,
        minute_point start_period, minute_point end_period,
        nlohmann::json j_data) {
        using namespace std::chrono_literals;
        interval_points interval;
        ohlc_prices ohlc;
        for (auto &[key, value] :
             j_data["Weekly Adjusted Time Series"].items()) {
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
            std::string st_open;
            std::string st_high;
            std::string st_low;
            std::string st_close;
            try {
                st_open = value["1. open"];
                st_high = value["2. high"];
                st_low = value["3. low"];
                st_close = value["4. close"];
            } catch (nlohmann::json::exception &e) {
                throw std::runtime_error("Fatal error: " +
                                         std::string(e.what()));
            }
            std::string::size_type sz; // alias of size_t
            double open, high, low, close;

            if (is_floating(st_open)) {
                open = std::stod(st_open, &sz);
            } else {
                return false;
            }
            if (is_floating(st_high)) {
                high = std::stod(st_high, &sz);
            } else {
                return false;
            }
            if (is_floating(st_low)) {
                low = std::stod(st_low, &sz);
            } else {
                return false;
            }
            if (is_floating(st_close)) {
                close = std::stod(st_close, &sz);
            } else {
                return false;
            }
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
            ohlc.set_prices(open, high, low, close);
            if (mp >= start_period && mp <= end_period) {
                hist[interval] = ohlc;
                to_serialize[interval_points_to_string(interval)] =
                    ohlc.to_string();
            } else {
                to_serialize[interval_points_to_string(interval)] =
                    ohlc.to_string();
            }
        }
        return true;
    }
    bool alphavantage_data_feed::set_monthly_data(
        price_map &hist, std::map<std::string, std::string> &to_serialize,
        minute_point start_period, minute_point end_period,
        nlohmann::json j_data) {
        using namespace std::chrono_literals;
        interval_points interval;
        ohlc_prices ohlc;
        for (auto &[key, value] :
             j_data["Monthly Adjusted Time Series"].items()) {
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
            std::string st_open;
            std::string st_high;
            std::string st_low;
            std::string st_close;
            try {
                st_open = value["1. open"];
                st_high = value["2. high"];
                st_low = value["3. low"];
                st_close = value["4. close"];
            } catch (nlohmann::json::exception &e) {
                throw std::runtime_error("Fatal error: " +
                                         std::string(e.what()));
            }
            std::string::size_type sz; // alias of size_t
            double open, high, low, close;

            if (is_floating(st_open)) {
                open = std::stod(st_open, &sz);
            } else {
                return false;
            }
            if (is_floating(st_high)) {
                high = std::stod(st_high, &sz);
            } else {
                return false;
            }
            if (is_floating(st_low)) {
                low = std::stod(st_low, &sz);
            } else {
                return false;
            }
            if (is_floating(st_close)) {
                close = std::stod(st_close, &sz);
            } else {
                return false;
            }
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
            ohlc.set_prices(open, high, low, close);
            if (mp_end >= start_period && mp_end <= end_period) {
                hist[interval] = ohlc;
                to_serialize[interval_points_to_string(interval)] =
                    ohlc.to_string();
            } else {
                to_serialize[interval_points_to_string(interval)] =
                    ohlc.to_string();
            }
        }
        return true;
    }

} // namespace portfolio