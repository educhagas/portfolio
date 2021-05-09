//
// Created by eduardo on 15/03/2021.
//

#ifndef PORTFOLIO_ALGORITHM_H
#define PORTFOLIO_ALGORITHM_H
#include "portfolio/data_feed/data_feed.h"
#include <chrono>
#include <cmath>
#include <string>

namespace portfolio {
    using minute_point = std::chrono::time_point<std::chrono::system_clock,
                                                 std::chrono::minutes>;
    using interval_points = std::pair<minute_point, minute_point>;

    /// \brief Test if string is in floating-point format.
    /// \param str_view String to be tested
    /// \return True if is in floating-point format or false otherwise.
    bool is_floating(std::string_view str_view);

    /// \brief Conversion from std::string to minute_point.
    /// \param str_mp std::string to be converted. To work correctly, a string
    /// in the format "%Y-%m-%d_%H-%M" must be used. \return minute_point
    /// converted from str_mp.
    minute_point string_to_minute_point(std::string_view str_mp);

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
    interval_points string_to_interval_points(std::string_view str_interval);

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

    template <class T>
    typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y, int precision = 5) {
        // the machine epsilon has to be scaled to the magnitude of the values
        // used and multiplied by the desired precision in ULPs (units in the
        // last place)
        return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() *
                                       std::fabs(x + y) * precision
               // unless the result is subnormal
               || std::fabs(x - y) < std::numeric_limits<T>::min();
    }
} // namespace portfolio
#endif // PORTFOLIO_ALGORITHM_H
