# This is a simple program that searches for the best parlay bets.

from itertools import combinations
from typing import List, Tuple
from analytics.line_compare import ConsolidatedOutcome
from pricing.odds import parlay_ev


def top_parlays(cands: List[ConsolidatedOutcome], k_legs_range=(2,5), top_n: int = 20):
    """Brute‑force small search across leg counts. Assumes independence for now.
    Returns list of (legs, offered_decimal, true_p, ev) sorted by EV desc.
    """
    ranked = []
    min_k, max_k = k_legs_range
    for L in range(min_k, max_k+1):
        for combo in combinations(cands, L):
            offered = [c.best_decimal for c in combo]
            true_ps = [c.est_true_p for c in combo]
            dec, tp, ev = parlay_ev(true_ps, offered)
            ranked.append((combo, dec, tp, ev))
    ranked.sort(key=lambda x: x[3], reverse=True)
    return ranked[:top_n]