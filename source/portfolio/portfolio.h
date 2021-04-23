//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_PORTFOLIO_H
#define PORTFOLIO_PORTFOLIO_H

#include <map>
#include <string>
namespace portfolio {
    class portfolio {
      public:
        // portfolio();
      protected:
        std::map<std::string, double> assets_;
        double total_risk_;
        double expected_return_;
    };
} // namespace portfolio
#endif // PORTFOLIO_PORTFOLIO_H
