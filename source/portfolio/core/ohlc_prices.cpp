//
// Created by eduardo on 20/03/2021.
//
#include "ohlc_prices.h"
#include "portfolio/common/algorithm.h"
#include <iomanip>
#include <string>
#include <vector>
namespace portfolio {

    double ohlc_prices::open() const { return open_price_; }
    double ohlc_prices::high() const { return high_price_; }
    double ohlc_prices::low() const { return low_price_; }
    double ohlc_prices::close() const { return close_price_; }
    ohlc_prices::ohlc_prices(double open, double high, double low,
                             double close) {
        open_price_ = open;
        high_price_ = high;
        low_price_ = low;
        close_price_ = close;
    }
    ohlc_prices::ohlc_prices() {
        open_price_ = 0.0;
        high_price_ = 0.0;
        low_price_ = 0.0;
        close_price_ = 0.0;
    }
    void ohlc_prices::set_prices(double open, double high, double low,
                                 double close) {
        open_price_ = open;
        high_price_ = high;
        low_price_ = low;
        close_price_ = close;
    }
    std::string ohlc_prices::to_string() const {
        std::string open_str = std::to_string(open_price_);
        std::string high_str = std::to_string(high_price_);
        std::string low_str = std::to_string(low_price_);
        std::string close_str = std::to_string(close_price_);
        return open_str + " " + high_str + " " + low_str + " " + close_str;
    }
    bool ohlc_prices::from_string(std::string_view str_ohlc) {
        std::string str(str_ohlc);
        std::istringstream iss(str);
        std::vector<std::string> result(std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>());
        std::string st_open = result[0];
        std::string st_high = result[1];
        std::string st_low = result[2];
        std::string st_close = result[3];

        std::string::size_type sz; // alias of size_t

        if (is_floating(st_open)) {
            open_price_ = std::stod(st_open, &sz);
        } else {
            return false;
        }
        if (is_floating(st_high)) {
            high_price_ = std::stod(st_high, &sz);
        } else {
            return false;
        }
        if (is_floating(st_low)) {
            low_price_ = std::stod(st_low, &sz);
        } else {
            return false;
        }
        if (is_floating(st_close)) {
            close_price_ = std::stod(st_close, &sz);
        } else {
            return false;
        }
        return true;
    }
    bool ohlc_prices::operator==(const ohlc_prices &rhs) const {
        return open_price_ == rhs.open_price_ &&
               high_price_ == rhs.high_price_ && low_price_ == rhs.low_price_ &&
               close_price_ == rhs.close_price_;
    }
    bool ohlc_prices::operator!=(const ohlc_prices &rhs) const {
        return !(rhs == *this);
    }

} // namespace portfolio