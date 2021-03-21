//
// Created by eduar on 03/02/2021.
//

#ifndef PORTFOLIO_ALPHAVANTAGE_DATA_FEED_H
#define PORTFOLIO_ALPHAVANTAGE_DATA_FEED_H

#include <map>
#include <nlohmann/json.hpp>
#include <portfolio/data_feed/data_feed.h>
namespace portfolio {
    class alphavantage_data_feed : public data_feed {
      public:
        /// Constructor of alphavatage_data_feed
        /// \param apiKey - create a api key for free on the website:
        /// https://www.alphavantage.co/
        explicit alphavantage_data_feed(const std::string_view &apiKey);

        data_feed_result fetch(std::string_view asset_code,
                               minute_point start_period,
                               minute_point end_period, timeframe tf) override;

      private:
        std::string_view api_key;
        std::string generate_url(std::string_view asset_code, timeframe tf);

        price_map request_online_data(std::string_view asset_code,
                                      minute_point start_period,
                                      minute_point end_period, timeframe tf);

        static void
        set_monthly_data(price_map &hist,
                         std::map<std::string, std::string> &to_serialize,
                         minute_point start_period, minute_point end_period,
                         nlohmann::json j_data);

        static void
        set_weekly_data(price_map &hist,
                        std::map<std::string, std::string> &to_serialize,
                        minute_point start_period, minute_point end_period,
                        nlohmann::json j_data);

        static void
        set_daily_data(price_map &hist,
                       std::map<std::string, std::string> &to_serialize,
                       minute_point start_period, minute_point end_period,
                       nlohmann::json j_data);
    };
} // namespace portfolio
#endif // PORTFOLIO_ALPHAVANTAGE_DATA_FEED_H
