# This is a simple program that compares the lines of two books for a given market.

from dataclasses import dataclass
from typing import List, Dict, Tuple
from pricing.odds import american_to_decimal, no_vig_from_american

@dataclass
class BookQuote:
    book: str
    market_id: str
    outcome: str
    american: int

@dataclass
class ConsolidatedOutcome:
    market_id: str
    outcome: str
    best_book: str
    best_american: int
    best_decimal: float
    # naive no‑vig estimate from a basket of books for the same market
    est_true_p: float


def consolidate_market(quotes: List[BookQuote]) -> List[ConsolidatedOutcome]:
    """Group quotes by (market_id, outcome). Take best price. Estimate true p via no‑vig across books for that market.
    Expect that for a given market_id, each outcome appears at least once across quotes.
    """
    from collections import defaultdict
    by_market: Dict[str, List[BookQuote]] = defaultdict(list)
    for q in quotes:
        by_market[q.market_id].append(q)

    results: List[ConsolidatedOutcome] = []
    for mid, qs in by_market.items():
        # outcomes in this market
        outcomes = sorted({(q.outcome) for q in qs})
        # for each outcome, pick best american (most favorable to bettor)
        best: Dict[str, Tuple[str, int, float]] = {}
        # collect a representative price per outcome (e.g., mean) for no‑vig
        rep_prices: Dict[str, int] = {}
        for o in outcomes:
            o_quotes = [q for q in qs if q.outcome == o]
            # best for bettor = highest decimal for positive odds, otherwise least negative american (max by decimal)
            best_q = max(o_quotes, key=lambda x: american_to_decimal(x.american))
            best[o] = (best_q.book, best_q.american, american_to_decimal(best_q.american))
            # use medium quote as representative (median american)
            rep_prices[o] = sorted([q.american for q in o_quotes])[len(o_quotes)//2]
        # estimate true probs by removing vig from representative lines
        americans = [rep_prices[o] for o in outcomes]
        true_ps = no_vig_from_american(americans)
        for o, p in zip(outcomes, true_ps):
            bbook, bamer, bdec = best[o]
            results.append(ConsolidatedOutcome(mid, o, bbook, bamer, bdec, p))
    return results