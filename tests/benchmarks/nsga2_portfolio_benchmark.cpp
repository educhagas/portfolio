//
// Created by eduar on 01/08/2021.
//
#include <benchmark/benchmark.h>

#include "portfolio/data_feed/alphavantage_data_feed.h"
#include "portfolio/data_feed/mock_data_feed.h"
#include "portfolio/evolutionary_algorithm.h"
#include "portfolio/portfolio.h"
#include <algorithm>
#include <random>
#include <vector>

static void generate_portfolio_nsga2(benchmark::State &state) {
    using namespace date::literals;
    using namespace std::chrono_literals;
    state.PauseTiming();
    std::vector<std::string> assets = {
        "ABEV3.SAO", "ALPA4.SAO",  "ALSO3.SAO",  "AMAR3.SAO",
        "AZUL4.SAO", // ASAI3.SAO
        "B3SA3.SAO", "BBAS3.SAO",  "BBDC3.SAO",  "BBDC4.SAO", "BBSE3.SAO",
        "BEEF3.SAO", "BPAC11.SAO", "BRAP4.SAO",  "BRDT3.SAO", "BRFS3.SAO",
        "BRKM5.SAO", "BRML3.SAO",  "BTOW3.SAO",  "CCRO3.SAO", "CESP6.SAO",
        "CIEL3.SAO", "CMIG4.SAO",  "COGN3.SAO",  "CPFE3.SAO", "CPLE6.SAO",
        "CRFB3.SAO", "CSAN3.SAO",  "CSMG3.SAO",  "CSNA3.SAO", "CVCB3.SAO",
        "CYRE3.SAO", "DTEX3.SAO",  "ECOR3.SAO",  "EGIE3.SAO", "ELET3.SAO",
        "ELET6.SAO", "EMBR3.SAO",  "ENBR3.SAO",  "ENEV3.SAO", "ENGI11.SAO",
        "EQTL3.SAO", "EZTC3.SAO",  "FLRY3.SAO",  "GGBR4.SAO", "GNDI3.SAO",
        "GOAU4.SAO", "GOLL4.SAO",  "HAPV3.SAO",  "HGTX3.SAO", "HYPE3.SAO",
        "IGTA3.SAO", "IRBR3.SAO",  "ITSA4.SAO",  "ITUB4.SAO", "JBSS3.SAO",
        "JHSF3.SAO", "KLBN11.SAO", "LAME3.SAO",  "LAME4.SAO", "LCAM3.SAO",
        "LIGT3.SAO", "LINX3.SAO",  "LREN3.SAO",  "MDIA3.SAO", //"LWSA3.SAO"
        "MEAL3.SAO", "MGLU3.SAO",  "MOVI3.SAO",  "MRFG3.SAO", "MRVE3.SAO",
        "MULT3.SAO", "NEOE3.SAO",  "NTCO3.SAO",  "PETR3.SAO", "PETR4.SAO",
        "PRIO3.SAO", //"PCAR3.SAO"
        "PSSA3.SAO", "QUAL3.SAO",  "RADL3.SAO",  "RAIL3.SAO", "RAPT4.SAO",
        "RENT3.SAO", "SANB11.SAO", "SAPR11.SAO", "SBSP3.SAO", "SULA11.SAO",
        "SUZB3.SAO", "TAEE11.SAO", "TIMS3.SAO",  "TOTS3.SAO", "TRPL4.SAO",
        "UGPA3.SAO", "USIM5.SAO",  "VALE3.SAO",  "VIVT3.SAO", "VVAR3.SAO",
        "WEGE3.SAO", "YDUQ3.SAO"};
    portfolio::minute_point mp_start =
        date::sys_days{2018_y / 01 / 01} + 10h + 0min;
    portfolio::minute_point mp_end =
        date::sys_days{2020_y / 12 / 31} + 18h + 0min;
    portfolio::minute_point start_interval =
        date::sys_days{2020_y / 01 / 06} + 10h + 0min;
    portfolio::minute_point end_interval =
        date::sys_days{2020_y / 01 / 10} + 18h + 0min;
    portfolio::interval_points interval =
        std::make_pair(start_interval, end_interval);
    portfolio::mock_data_feed mock_df;
    // portfolio::alphavantage_data_feed af("demo", true);
    portfolio::market_data md(assets, mock_df, mp_start, mp_end,
                              portfolio::timeframe::weekly, interval, 12);
    //    portfolio::market_data md(assets, af, mp_start, mp_end,
    //                              portfolio::timeframe::weekly, interval, 12);
    portfolio::portfolio port(md);
    portfolio::evolutionary_algorithm solver(md);
    solver.algorithm(portfolio::evolutionary_algorithm::algorithm::NSGA2);
    solver.population_size(300);
    state.ResumeTiming();
    for (auto _ : state) {
        solver.max_generations(state.range(0));
        solver.nsga2_run();
    }
}

BENCHMARK(generate_portfolio_nsga2)
    ->Arg(10)
    ->Arg(25)
    ->Arg(50)
    ->Arg(75)
    ->Arg(100);

BENCHMARK_MAIN();