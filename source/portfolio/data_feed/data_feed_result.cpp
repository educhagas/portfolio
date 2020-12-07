//
// Created by Alan Freitas on 12/7/20.
//

#include "data_feed_result.h"

namespace portfolio {
    double data_feed_result::current_price() const {
        return current_price_;
    }

    data_feed_result::data_feed_result(double current_price) : current_price_(current_price) {}
}