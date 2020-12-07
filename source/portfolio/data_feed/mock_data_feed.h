//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_MOCK_DATA_FEED_H
#define PORTFOLIO_MOCK_DATA_FEED_H

#include <portfolio/data_feed/data_feed.h>

namespace portfolio {
    class mock_data_feed : public data_feed {
    public:
        data_feed_result fetch(std::string_view) override;
    };
}


#endif //PORTFOLIO_MOCK_DATA_FEED_H
