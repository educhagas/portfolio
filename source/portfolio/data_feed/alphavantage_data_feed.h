//
// Created by eduardo on 03/02/2021.
//

#ifndef PORTFOLIO_ALPHAVANTAGE_DATA_FEED_H
#define PORTFOLIO_ALPHAVANTAGE_DATA_FEED_H

#include <map>
#include <nlohmann/json.hpp>
#include <portfolio/data_feed/data_feed.h>
namespace portfolio {
    class alphavantage_data_feed : public data_feed {
      public:
        /// \brief Constructor of alphavantage_data_feed
        /// \param apiKey - API key used to access data from:
        /// https://www.alphavantage.co/
        /// \param api_key_is_free Indicates whether API key is free or not. If
        /// it is free, it restricts a maximum of 5 requests per minute.
        explicit alphavantage_data_feed(const std::string_view &apiKey,
                                        bool api_key_is_free);

        /// \brief Fetch price data from alphavantage and saves it in price_map.
        /// \param start_period Initial minute_point.
        /// \param end_period Final minute_point.
        /// \param tf Timeframe used on request.
        /// \return Price_map "filled" according to the input parameters.
        data_feed_result fetch(std::string_view asset_code,
                               minute_point start_period,
                               minute_point end_period, timeframe tf) override;

      private:
        /// Generates url to download data from alphavantage.
        /// \param asset_code Symbol of asset. For B3 assets_proportions_ add
        /// ".SAO" after the code. Example: "PETR4.SAO". \param tf Timeframe
        /// used on url./// \return URL used to download alphavantage data.
        std::string generate_url(std::string_view asset_code, timeframe tf);

        /// \brief Access and download alphavantage data.
        /// \param historical_data Price_map where the query data will be saved.
        /// \param asset_code Symbol of asset. For B3 assets_proportions_ add
        /// ".SAO" after the code. Example: "PETR4.SAO". \param start_period
        /// Initial/// minute_point. \param end_period Final minute_point.
        /// \param tf Timeframe used on request. \return Price_map "filled"
        /// according to the input parameters.
        bool request_online(price_map &historical_data,
                            std::string_view asset_code,
                            minute_point start_period, minute_point end_period,
                            timeframe tf);

        /// \brief Handles data received from alphavantage for a monthly
        /// timeframe by saving it to a price_map and preparing the data to be
        /// serialized. \param hist Price_map where the query data will be
        /// saved. \param to_serialize Map used in the data serialization step.
        /// \param start_period Initial minute_point.
        /// \param end_period Final minute_point.
        /// \param j_data Data received from alphavantage.
        /// \return True if not occurs errors or false otherwise.
        static bool
        set_monthly_data(price_map &hist,
                         std::map<std::string, std::string> &to_serialize,
                         minute_point start_period, minute_point end_period,
                         nlohmann::json j_data);

        /// \brief Handles data received from alphavantage for a weekly
        /// timeframe by saving it to a price_map and preparing the data to be
        /// serialized. \param hist Price_map where the query data will be
        /// saved. \param to_serialize Map used in the data serialization step.
        /// \param start_period Initial minute_point.
        /// \param end_period Final minute_point.
        /// \param j_data Data received from alphavantage.
        /// \return True if not occurs errors or false otherwise.
        static bool
        set_weekly_data(price_map &hist,
                        std::map<std::string, std::string> &to_serialize,
                        minute_point start_period, minute_point end_period,
                        nlohmann::json j_data);

        /// \brief Handles data received from alphavantage for a daily timeframe
        /// by saving it to a price_map and preparing the data to be serialized.
        /// \param hist Price_map where the query data will be saved.
        /// \param to_serialize Map used in the data serialization step.
        /// \param start_period Initial minute_point.
        /// \param end_period Final minute_point.
        /// \param j_data Data received from alphavantage.
        /// \return True if not occurs errors or false otherwise.
        static bool
        set_daily_data(price_map &hist,
                       std::map<std::string, std::string> &to_serialize,
                       minute_point start_period, minute_point end_period,
                       nlohmann::json j_data);

        std::string_view api_key_;
        bool api_key_is_free_;
        std::chrono::time_point<std::chrono::system_clock> last_request_tp_;
    };
} // namespace portfolio
#endif // PORTFOLIO_ALPHAVANTAGE_DATA_FEED_H
