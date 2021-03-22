//
// Created by eduar on 15/03/2021.
//

#ifndef PORTFOLIO_OHLC_PRICES_H
#define PORTFOLIO_OHLC_PRICES_H
#include <string>
#include <tuple>
namespace portfolio {
    class ohlc_prices {
      public:
        ohlc_prices();
        /// \brief Class constructor.
        /// \param data std::tuple<double,double,double,double> for open, high,
        /// low and close prices.
        explicit ohlc_prices(std::tuple<double, double, double, double> data);

        /// \brief Class constructor.
        /// \param open Open price.
        /// \param high High price.
        /// \param low Low price.
        /// \param close Close price.
        ohlc_prices(double open, double high, double low, double close);

        bool operator==(const ohlc_prices &rhs) const;
        bool operator!=(const ohlc_prices &rhs) const;

        /// \brief Get open price.
        /// \return Double - open price.
        double open();

        /// \brief Get high price.
        /// \return Double - high price.
        double high();

        /// \brief Get low price.
        /// \return Double - low price.
        double low();

        /// \brief Get close price.
        /// \return Double - close price.
        double close();

        /// \brief Set OHLC prices.
        /// \param open Open price.
        /// \param high High price.
        /// \param low Low price.
        /// \param close Close price.
        void set_prices(double open, double high, double low, double close);

        /// \brief Convert OHLC to string.
        /// \return OHLC prices converted to std::string. Output string in
        /// "double double double double" format(Space-separated values).
        std::string to_string();

        /// \brief Set values from OHLC from std::string.
        /// \param str_ohlc Values of OHLC prices. To work correctly, a string
        /// in the format "double double double double" must be used.
        void from_string(const std::string &str_ohlc);

      private:
        std::tuple<double, double, double, double> data_;
    };
} // namespace portfolio
#endif // PORTFOLIO_OHLC_PRICES_H
