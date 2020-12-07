//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_DATA_FEED_RESULT_H
#define PORTFOLIO_DATA_FEED_RESULT_H

namespace portfolio {
    class data_feed_result {
    public /* constructors */:
        explicit data_feed_result(double currentPrice);

    public /* getters and setters */:
        [[nodiscard]] double current_price() const;

    private:
        double current_price_;
    };
}

#endif //PORTFOLIO_DATA_FEED_RESULT_H
