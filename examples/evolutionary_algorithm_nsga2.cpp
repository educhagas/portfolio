//
// Created by eduar on 21/09/2021.
//
#include "portfolio/data_feed/alphavantage_data_feed.h"
#include "portfolio/data_feed/mock_data_feed.h"
#include "portfolio/evolutionary_algorithm.h"
#include "portfolio/matplot/pareto_front.h"
#include "portfolio/problem/problem.h"
#include "portfolio/problem/return/portfolio_return_mean.h"
#include "portfolio/problem/risk/portfolio_risk_mad.h"
int main() {
    using namespace date::literals;
    using namespace std::chrono_literals;
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
        date::sys_days{2021_y / 06 / 07} + 10h + 0min;
    portfolio::minute_point mp_end =
        date::sys_days{2022_y / 01 / 21} + 18h + 0min;

    portfolio::minute_point start_interval =
        date::sys_days{2022_y / 01 / 10} + 10h + 0min;
    portfolio::minute_point end_interval =
        date::sys_days{2022_y / 01 / 14} + 18h + 0min;

    portfolio::interval_points interval =
        std::make_pair(start_interval, end_interval);
    // portfolio::mock_data_feed mock_df;
    portfolio::alphavantage_data_feed af("demo", false);
    size_t periods = 12;
    //    portfolio::market_data md(assets, mock_df, mp_start, mp_end,
    //                              portfolio::timeframe::weekly);
    portfolio::market_data md(assets, af, mp_start, mp_end,
                              portfolio::timeframe::weekly);
    std::shared_ptr<portfolio::portfolio_return_mean> pm(
        new portfolio::portfolio_return_mean(md, interval, periods));
    std::shared_ptr<portfolio::portfolio_risk_mad> pr(
        new portfolio::portfolio_risk_mad(md, interval, periods));
    portfolio::problem prob(pr, pm, md, interval);
    std::ofstream fout("test_k30_50.txt");
    pareto::front<double, 2, portfolio::portfolio> pareto_front_result;
    pareto_front_result = pareto::front<double, 2, portfolio::portfolio>(
        {pareto::min, pareto::max});
    for (size_t i = 0; i < 10; ++i) { // 100
        fout << i << "\n";
        portfolio::portfolio port(prob);
        portfolio::evolutionary_algorithm solver(prob);
        solver.algorithm(portfolio::evolutionary_algorithm::algorithm::NSGA2);
        solver.population_size(500); // 500
        solver.max_generations(500); // 500
        auto start = std::chrono::system_clock::now();
        solver.run();
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end - start;
        fout << diff.count() << "\n";
        fout << solver.pareto_front().hypervolume() << "\n";
        fout << solver.pareto_front().size() << "\n";
        for (auto &p : solver.pareto_front()) {
            fout << p.first.values()[0] << " ; " << p.first.values()[1] << "\n";
            pareto_front_result.insert(std::make_pair(
                pareto::front<double, 2, portfolio::portfolio>::key_type(
                    {p.first.values()[0], p.first.values()[1]}),
                p.second));
        }
    }
    fout.close();
    std::ofstream fout2("test_k30_50_result.txt");
    fout2 << pareto_front_result.hypervolume() << "\n";
    fout2 << pareto_front_result.size() << "\n";
    for (auto p : pareto_front_result) {
        fout2 << p.first.values()[0] << " ; " << p.first.values()[1] << "\n";
    }
    fout2.close();
    // portfolio::save_plot(solver.pareto_front(), "plot.jpg");
}