//
// Created by eduar on 20/03/2021.
//
#include "ohlc_prices.h"
#include "portfolio/common/algorithm.h"
#include <iomanip>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
namespace portfolio {

    ohlc_prices::ohlc_prices(std::tuple<double, double, double, double> data)
        : data_(std::move(data)) {}
    bool ohlc_prices::operator==(const ohlc_prices &rhs) const {
        return data_ == rhs.data_;
    }
    bool ohlc_prices::operator!=(const ohlc_prices &rhs) const {
        return !(rhs == *this);
    }
    double ohlc_prices::open() { return std::get<0>(data_); }
    double ohlc_prices::high() { return std::get<1>(data_); }
    double ohlc_prices::low() { return std::get<2>(data_); }
    double ohlc_prices::close() { return std::get<3>(data_); }
    ohlc_prices::ohlc_prices(double open, double high, double low, double close)
        : data_(std::make_tuple(open, high, low, close)) {}
    ohlc_prices::ohlc_prices() : data_(std::make_tuple(0.0, 0.0, 0.0, 0.0)) {}
    void ohlc_prices::set_prices(double open, double high, double low,
                                 double close) {
        data_ = std::make_tuple(open, high, low, close);
    }
    std::string ohlc_prices::to_string() {
        std::string open_str = std::to_string(this->open());
        std::string high_str = std::to_string(this->high());
        std::string low_str = std::to_string(this->low());
        std::string close_str = std::to_string(this->close());
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

} // namespace portfolio