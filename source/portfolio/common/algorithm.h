//
// Created by eduar on 15/03/2021.
//

#ifndef PORTFOLIO_ALGORITHM_H
#define PORTFOLIO_ALGORITHM_H
#include "portfolio/data_feed/data_feed.h"
#include <chrono>
#include <string>

namespace portfolio {
    using minute_point = std::chrono::time_point<std::chrono::system_clock,
                                                 std::chrono::minutes>;
    using interval_points = std::pair<minute_point, minute_point>;

    /// \brief Conversion from std::string to double making exception treatment.
    /// \param str std::string to be converted.
    /// \return double converted from std::string.
    double string_to_double(const std::string &str);

    /// \brief Conversion from std::string to minute_point.
    /// \param str_mp std::string to be converted. To work correctly, a string
    /// in the format "%Y-%m-%d_%H-%M" must be used. \return minute_point
    /// converted from str_mp.
    minute_point string_to_minute_point(std::string &str_mp);

    /// \brief Conversion from minute_point to std::string.
    /// \param mp minute_point to be converted.
    /// \return std::string converted from mp. Output string in "%Y-%m-%d_%H-%M"
    /// format.
    std::string minute_point_to_string(minute_point mp);

    /// \brief Conversion from interval_points to std::string.
    /// \param interval interval_points to be converted.
    /// \return std::string converted from interval. Output string in
    /// "%Y-%m-%d_%H-%M|%Y-%m-%d_%H-%M" format.
    std::string interval_points_to_string(interval_points interval);

    /// \brief Conversion from std::string to interval_points.
    /// \param str_interval std::string to be converted. To work correctly, a
    /// string in the format "%Y-%m-%d_%H-%M|%Y-%m-%d_%H-%M" must be used.
    /// \return interval_points converted from str_interval.
    interval_points string_to_interval_points(std::string str_interval);

    /// \brief Defines a name for local data file.
    /// \param asset_code Symbol of asset.
    /// \param start_period Initial time point.
    /// \param end_period End time point.
    /// \param tf Timeframe of data.
    /// \return Name used for save data file.
    std::string set_filename(std::string_view asset_code,
                             std::string_view start_period,
                             std::string_view end_period, timeframe tf);

    /// \brief Defines the initial part of the name for the local data file.
    /// \param asset_code Symbol of asset.
    /// \param tf Timeframe of data.
    /// \return Initial part of local data file used to search for this file.
    std::string start_filename(std::string_view asset_code, timeframe tf);
} // namespace portfolio
#endif // PORTFOLIO_ALGORITHM_H
