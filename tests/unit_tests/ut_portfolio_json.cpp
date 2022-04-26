#define CATCH_CONFIG_MAIN

#include "portfolio/common/algorithm.h"
#include "portfolio/data_feed/alphavantage_data_feed.h"
#include "portfolio/data_feed/mock_data_feed.h"
#include "portfolio/evolutionary_algorithm.h"
#include "portfolio/json/json.h"
#include "portfolio/market_data.h"
#include "portfolio/portfolio.h"
#include "portfolio/problem/problem.h"
#include "portfolio/problem/return/portfolio_return.h"
#include "portfolio/problem/return/portfolio_return_mean.h"
#include "portfolio/problem/risk/portfolio_risk_mad.h"
#include <catch2/catch.hpp>
#include <chrono>
#include <filesystem>

TEST_CASE("Portfolio and Market Data") {
    using namespace date::literals;
    using namespace std::chrono_literals;
    //    std::vector<std::string> assets = {
    //        "PETR4.SAO", "VALE3.SAO", "ITUB4.SAO", "ABEV3.SAO", "BBDC4.SAO",
    //        "BBAS3.SAO", "CMIG4.SAO", "ELET3.SAO", "ITSA4.SAO", "MGLU3.SAO"};
    std::vector<std::string> assets = {
        "RRRP3.SAO",  "ALPA4.SAO",  "ABEV3.SAO",  "AMER3.SAO",  "ASAI3.SAO",
        "AZUL4.SAO",  "B3SA3.SAO",  "BIDI11.SAO", "BPAN4.SAO",  "BBSE3.SAO",
        "BRML3.SAO",  "BBDC3.SAO",  "BBDC4.SAO",  "BRAP4.SAO",  "BBAS3.SAO",
        "BRKM5.SAO",  "BRFS3.SAO",  "BPAC11.SAO", "CRFB3.SAO",  "CCRO3.SAO",
        "CMIG4.SAO",  "CESP6.SAO",  "CIEL3.SAO",  "COGN3.SAO",  "CPLE6.SAO",
        "CSAN3.SAO",  "CPFE3.SAO",  "CMIN3.SAO",  "CVCB3.SAO",  "CYRE3.SAO",
        "DXCO3.SAO",  "ECOR3.SAO",  "ELET3.SAO",  "ELET6.SAO",  "EMBR3.SAO",
        "ENBR3.SAO",  "ENGI11.SAO", "ENEV3.SAO",  "EGIE3.SAO",  "EQTL3.SAO",
        "EZTC3.SAO",  "FLRY3.SAO",  "GGBR4.SAO",  "GOAU4.SAO",  "GOLL4.SAO",
        "NTCO3.SAO",  "SOMA3.SAO",  "HAPV3.SAO",  "HYPE3.SAO", //"IGTI11.SAO",
        "GNDI3.SAO",  "IRBR3.SAO",  "ITSA4.SAO",  "ITUB4.SAO",  "JBSS3.SAO",
        "KLBN11.SAO", "LIGT3.SAO",  "RENT3.SAO",  "LCAM3.SAO",  "LWSA3.SAO",
        "LAME3.SAO",  "LAME4.SAO",  "AMAR3.SAO",  "LREN3.SAO",  "MGLU3.SAO",
        "MRFG3.SAO",  "CASH3.SAO",  "BEEF3.SAO",  "MOVI3.SAO",  "MRVE3.SAO",
        "MULT3.SAO",  "PCAR3.SAO",  "PETR3.SAO",  "PETR4.SAO",  "PRIO3.SAO",
        "PETZ3.SAO",  "PSSA3.SAO",  "POSI3.SAO",  "QUAL3.SAO",  "RADL3.SAO",
        "RDOR3.SAO",  "RAIL3.SAO",  "SBSP3.SAO",  "SANB11.SAO", "STBP3.SAO",
        "CSNA3.SAO",  "SLCE3.SAO",  "SULA11.SAO", "SUZB3.SAO",  "TAEE11.SAO",
        "VIVT3.SAO",  "TIMS3.SAO",  "TOTS3.SAO",  "UGPA3.SAO",  "USIM5.SAO",
        "VALE3.SAO",  "VIIA3.SAO",  "VBBR3.SAO",  "WEGE3.SAO",  "YDUQ3.SAO"};
    portfolio::minute_point mp_start =
        date::sys_days{2018_y / 01 / 01} + 10h + 0min;
    portfolio::minute_point mp_end =
        date::sys_days{2020_y / 12 / 31} + 18h + 0min;
    portfolio::minute_point start_interval =
        date::sys_days{2020_y / 01 / 06} + 10h + 0min;
    portfolio::minute_point end_interval =
        date::sys_days{2020_y / 01 / 06} + 18h + 0min;
    portfolio::interval_points interval =
        std::make_pair(start_interval, end_interval);
    portfolio::mock_data_feed mock_df;
    portfolio::market_data md(assets, mock_df, mp_start, mp_end,
                              portfolio::timeframe::daily);
    size_t periods = 12;
    std::shared_ptr<portfolio::portfolio_return_mean> pm(
        new portfolio::portfolio_return_mean(md, interval, periods));
    std::shared_ptr<portfolio::portfolio_risk_mad> port_mad(
        new portfolio::portfolio_risk_mad(md, interval, periods));
    portfolio::problem prob(port_mad, pm, md, interval);
    std::string path = "./";
    bool data_folder_exists = false;
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        std::string filename(entry.path().filename().string());
        if (filename == "port_save") {
            data_folder_exists = true;
            break;
        }
    }
    if (!data_folder_exists) {
        std::filesystem::create_directory("./port_save");
    }

    portfolio::portfolio port(prob);
    SECTION("Portfolio save and restore") {
        double upper_bound = port.upper_bound();
        double lower_bound = port.lower_bound();
        size_t k = port.k();
        portfolio::portfolio_to_json(port, "./port_save/port_test.json");

        portfolio::portfolio port2 =
            portfolio::from_json("./port_save/port_test.json");

        // if file exists, not throw exeption
        REQUIRE_NOTHROW(portfolio::from_json("./port_save/port_test.json"));

        // if file not exists, throws exception
        REQUIRE_THROWS(portfolio::from_json("./port_save/port_test2.json"));

        REQUIRE(portfolio::almost_equal(port.capital(), port2.capital()));
        REQUIRE(
            portfolio::almost_equal(port.upper_bound(), port2.upper_bound()));
        REQUIRE(
            portfolio::almost_equal(port.lower_bound(), port2.lower_bound()));
        REQUIRE(port.k() == port2.k());

        for (auto a = port.assets_proportions_begin();
             a != port.assets_proportions_end(); ++a) {
            REQUIRE(portfolio::almost_equal(port.asset_proportion(a->first),
                                            port2.asset_proportion(a->first)));
            REQUIRE(portfolio::almost_equal(port.asset_price(a->first),
                                            port2.asset_price(a->first)));
        }
    }
}