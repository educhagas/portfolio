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
    void ohlc_prices::from_string(const std::string &str_ohlc) {
        std::istringstream iss(str_ohlc);
        std::vector<std::string> result(std::istream_iterator<std::string>{iss},
                                        std::istream_iterator<std::string>());
        double open = string_to_double(result[0]);
        double high = string_to_double(result[1]);
        double low = string_to_double(result[2]);
        double close = string_to_double(result[3]);
        this->set_prices(open, high, low, close);
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