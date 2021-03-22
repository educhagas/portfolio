//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_MOCK_DATA_FEED_H
#define PORTFOLIO_MOCK_DATA_FEED_H

#include <chrono>
#include <date/date.h>
#include <portfolio/data_feed/data_feed.h>

namespace portfolio {
    class mock_data_feed : public data_feed {
      public:
        /// \brief Generates random price data and saves it in data_feed_result.
        /// \param asset_code Symbol of asset.
        /// \param start_period Initial minute_point.
        /// \param end_period Final minute_point.
        /// \param tf Timeframe used on request.
        /// \return Data_feed_result "filled" according to the input parameters.
        data_feed_result fetch(std::string_view asset_code,
                               minute_point start_period,
                               minute_point end_period, timeframe tf) override;

      private:
        /// \brief Generates random price data and saves it in price_map.
        /// \param start_period Initial minute_point.
        /// \param end_period Final minute_point.
        /// \param tf Timeframe used on request.
        /// \return Price_map "filled" according to the input parameters.
        static price_map generate_historical_data(minute_point start_period,
                                                  minute_point end_period,
                                                  timeframe tf);
        /// \brief Fill in price_map when using intraday or daily timeframes.
        /// \param start_period Initial minute_point.
        /// \param end_period Final minute_point.
        /// \param tf Timeframe used on request.
        /// \return Price_map "filled" according to the input parameters.
        static price_map daily_intraday(minute_point start_period,
                                        minute_point end_period, timeframe tf);

        /// \brief Fill in price_map when using weekly timeframe.
        /// \param start_period Initial minute_point.
        /// \param end_period Final minute_point.
        /// \return Price_map "filled" according to the input parameters.
        static price_map weekly(minute_point start_period,
                                minute_point end_period);

        /// \brief Fill in price_map when using monthly timeframe.
        /// \param start_period Initial minute_point.
        /// \param end_period Final minute_point.
        /// \return Price_map "filled" according to the input parameters.
        static price_map monthly(minute_point start_period,
                                 minute_point end_period);

        /// \brief Calculates increment for interval_points based on timeframe.
        /// \param tf Timeframe used for increment.
        /// \return Increment value in minutes for tf.
        static std::chrono::minutes increment_by(timeframe tf);
    };
} // namespace portfolio

#endif // PORTFOLIO_MOCK_DATA_FEED_H