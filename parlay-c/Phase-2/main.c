#include <stdio.h>
#include "parlay.h"

int main() {
    Outcome outcomes[] = {
        {1, "Game1", "Spread", "TeamA", "Book1", 1.91, 0.55},
        {2, "Game2", "Total", "Over", "Book2", 2.05, 0.52},
        {3, "Game3", "Moneyline", "TeamB", "Book1", 1.80, 0.60},
        {4, "Game4", "Spread", "TeamC", "Book3", 2.10, 0.50},
    };
    int n = sizeof(outcomes) / sizeof(outcomes[0]);

    build_parlays(outcomes, n, 2, 4);
    return 0;
}