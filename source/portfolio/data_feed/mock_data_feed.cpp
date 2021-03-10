//
// Created by Alan Freitas on 12/7/20.
//

#include "mock_data_feed.h"
#include <random>

namespace portfolio {

    std::chrono::minutes
    mock_data_feed::calculate_increment(portfolio::timeframe tf) {
        using namespace std::chrono_literals;
        switch (tf) {
        case timeframe::minutes_15:
            return 15min;
        case timeframe::daily:
            return 8h;
        case timeframe::hourly:
            return 60min;
        case timeframe::monthly:
            return 31 * 24h;
        case timeframe::weekly:
            return 7 * 24h;
        default:
            return 24h;
        }
    }

    data_feed_result mock_data_feed::fetch(std::string_view asset_code,
                                           minute_point start_period,
                                           minute_point end_period,
                                           timeframe tf) {
        return data_feed_result(
            generate_historical_data2(start_period, end_period, tf));
    }
    price_map mock_data_feed::generate_historical_data2(
        minute_point start_period, minute_point end_period, timeframe tf) {
        price_map historical_data;
        switch (tf) {
        case (timeframe::monthly):
            historical_data =
                genetate_historical_data_monthly(start_period, end_period);
            break;
        case (timeframe::weekly):
            historical_data =
                generate_historical_data_weekly(start_period, end_period);
            break;
        default:
            historical_data = generate_historical_data_daily_intraday(
                start_period, end_period, tf);
        }
        return historical_data;
    }
    price_map mock_data_feed::generate_historical_data_daily_intraday(
        minute_point start_period, minute_point end_period, timeframe tf) {
        std::chrono::minutes increment = calculate_increment(tf);
        price_map historical_data;
        static std::random_device r;
        static std::default_random_engine generator(r());
        std::uniform_int_distribution<int> ud_int(10, 50);
        std::uniform_real_distribution<double> ud_double(1, 2);
        // initial price between 10.00 and 100.00
        double open_price = ud_int(generator) * ud_double(generator);
        double low_price;
        double high_price;
        double close_price;
        for (minute_point i = start_period; i <= end_period;
             i = i + increment) {
            day_point dp = date::floor<date::days>(i);
            using namespace date;
            using namespace std::chrono_literals;
            if (dp != date::Saturday &&
                dp != date::Sunday) { // if is not weekend
                auto time = date::make_time(
                    std::chrono::duration_cast<std::chrono::minutes>(i - dp));
                date::hh_mm_ss<std::chrono::seconds> start_time(10h + 0min);
                date::hh_mm_ss<std::chrono::seconds> end_time(18h + 0min);
                if (time.to_duration() >= start_time.to_duration() &&
                    time.to_duration() < end_time.to_duration()) {
                    int side = ud_int(generator);
                    double volatility = (ud_double(generator) - 1) * 5;
                    side < 30 ? close_price = open_price -
                                              (open_price * (volatility / 100))
                              : close_price = open_price +
                                              (open_price * (volatility / 100));
                    if (open_price < close_price) {
                        volatility = (ud_double(generator) - 1) * 2;
                        low_price =
                            open_price - (open_price * (volatility / 100));
                        volatility = (ud_double(generator) - 1) * 2;
                        high_price =
                            close_price + (close_price * (volatility / 100));
                    } else {
                        volatility = (ud_double(generator) - 1) * 2;
                        low_price =
                            close_price - (close_price * (volatility / 100));
                        volatility = (ud_double(generator) - 1) * 2;
                        high_price =
                            open_price + (open_price * (volatility / 100));
                    }
                    historical_data[std::make_pair(i, i + increment)] =
                        std::make_tuple(open_price, high_price, low_price,
                                        close_price);
                    open_price = close_price;
                }
            }
        }
        return historical_data;
    }
    price_map
    mock_data_feed::generate_historical_data_weekly(minute_point start_period,
                                                    minute_point end_period) {
        price_map historical_data;
        using namespace std::chrono_literals;
        day_point dp_start = date::floor<date::days>(start_period);
        day_point dp_end = date::floor<date::days>(end_period);
        while (dp_start != date::Monday) {
            dp_start = dp_start - date::days(1);
        }
        while (dp_end != date::Friday) {
            dp_end = dp_end + date::days(1);
        }
        static std::random_device r;
        static std::default_random_engine generator(r());
        std::uniform_int_distribution<int> ud_int(10, 50);
        std::uniform_real_distribution<double> ud_double(1, 2);
        // initial price between 10.00 and 100.00
        double open_price = ud_int(generator) * ud_double(generator);
        double low_price;
        double high_price;
        for (day_point i = dp_start; i < dp_end; i = i + date::days(7)) {
            minute_point start = i + 10h;
            minute_point end = i + date::days(4) + 18h;
            int side = ud_int(generator);
            double volatility = (ud_double(generator) - 1) * 5;
            double close_price;
            side < 30
                ? close_price = open_price - (open_price * (volatility / 100))
                : close_price = open_price + (open_price * (volatility / 100));
            if (open_price < close_price) {
                volatility = (ud_double(generator) - 1) * 2;
                low_price = open_price - (open_price * (volatility / 100));
                volatility = (ud_double(generator) - 1) * 2;
                high_price = close_price + (close_price * (volatility / 100));
            } else {
                volatility = (ud_double(generator) - 1) * 2;
                low_price = close_price - (close_price * (volatility / 100));
                volatility = (ud_double(generator) - 1) * 2;
                high_price = open_price + (open_price * (volatility / 100));
            }
            historical_data[std::make_pair(start, end)] =
                std::make_tuple(open_price, high_price, low_price, close_price);
            open_price = close_price;
        }
        return historical_data;
    }
    price_map
    mock_data_feed::genetate_historical_data_monthly(minute_point start_period,
                                                     minute_point end_period) {
        price_map historical_data;
        date::year_month_day date_start = date::floor<date::days>(start_period);
        date::year_month_day date_end = date::floor<date::days>(end_period);
        date_start =
            date::year_month_day{date_start.year() / date_start.month() / 1};
        date_end = date::year_month_day{date_end.year() / date_end.month() /
                                        date::last};
        static std::random_device r;
        static std::default_random_engine generator(r());
        std::uniform_int_distribution<int> ud_int(10, 50);
        std::uniform_real_distribution<double> ud_double(1, 2);
        // initial price between 10.00 and 100.00
        double open_price = ud_int(generator) * ud_double(generator);
        double low_price;
        double high_price;
        using namespace std::chrono_literals;
        for (day_point i = date_start; i < date_end; i = i + date::days(31)) {
            date::year_month_day day = i;
            minute_point start =
                date::sys_days{day.year() / day.month() / 1} + 10h;
            minute_point end =
                date::sys_days{day.year() / day.month() / date::last} + 18h;
            int side = ud_int(generator);
            double volatility = (ud_double(generator) - 1) * 5;
            double close_price;
            side < 30
                ? close_price = open_price - (open_price * (volatility / 100))
                : close_price = open_price + (open_price * (volatility / 100));
            if (open_price < close_price) {
                volatility = (ud_double(generator) - 1) * 2;
                low_price = open_price - (open_price * (volatility / 100));
                volatility = (ud_double(generator) - 1) * 2;
                high_price = close_price + (close_price * (volatility / 100));
            } else {
                volatility = (ud_double(generator) - 1) * 2;
                low_price = close_price - (close_price * (volatility / 100));
                volatility = (ud_double(generator) - 1) * 2;
                high_price = open_price + (open_price * (volatility / 100));
            }
            historical_data[std::make_pair(start, end)] =
                std::make_tuple(open_price, high_price, low_price, close_price);
            open_price = close_price;
        }
        return historical_data;
    }
} // namespace portfolio