//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_DATA_FEED_H
#define PORTFOLIO_DATA_FEED_H

#include <string_view>
#include <portfolio/data_feed/data_feed_result.h>

namespace portfolio {
    class data_feed {
    public:
        virtual data_feed_result fetch(std::string_view asset_code) = 0;
    };
}


#endif //PORTFOLIO_DATA_FEED_H
