//
// Created by eduardo on 15/03/2021.
//

#ifndef PORTFOLIO_OHLC_PRICES_H
#define PORTFOLIO_OHLC_PRICES_H
#include <string>
namespace portfolio {
    class ohlc_prices {
      public:
        ohlc_prices();

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
        [[nodiscard]] double open() const;

        /// \brief Get high price.
        /// \return Double - high price.
        [[nodiscard]] double high() const;

        /// \brief Get low price.
        /// \return Double - low price.
        [[nodiscard]] double low() const;

        /// \brief Get close price.
        /// \return Double - close price.
        [[nodiscard]] double close() const;

        /// \brief Set OHLC prices.
        /// \param open Open price.
        /// \param high High price.
        /// \param low Low price.
        /// \param close Close price.
        void set_prices(double open, double high, double low, double close);

        /// \brief Convert OHLC to string.
        /// \return OHLC prices converted to std::string. Output string in
        /// "double double double double" format(Space-separated values).
        [[nodiscard]] std::string to_string() const;

        /// \brief Set values from OHLC from std::string.
        /// \param str_ohlc Values of OHLC prices. To work correctly, a string
        /// in the format "double double double double" must be used.
        void from_string(const std::string &str_ohlc);

      private:
        double open_price_;
        double high_price_;
        double low_price_;
        double close_price_;
    };
} // namespace portfolio
#endif // PORTFOLIO_OHLC_PRICES_H
