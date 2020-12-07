#include <matplot/matplot.h>
#include <vector>

int main() {
    using namespace matplot;
    plot(std::vector{1, 2, 3, 4, 5, 6});
    show();
    return 0;
}