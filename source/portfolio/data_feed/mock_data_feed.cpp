//
// Created by Alan Freitas on 12/7/20.
//

#include "mock_data_feed.h"

namespace portfolio {
    data_feed_result mock_data_feed::fetch(std::string_view asset) {
        return data_feed_result(27.41);
    }
}