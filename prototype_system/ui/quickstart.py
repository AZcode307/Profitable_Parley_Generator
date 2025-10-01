from ingest.csv_loader import load_quotes_csv
from analytics.line_compare import consolidate_market
from portfolio.parlay_search import top_parlays

if __name__ == "__main__":
    quotes = load_quotes_csv("sample_quotes.csv")
    cands = consolidate_market(quotes)
    results = top_parlays(cands, (2,4), top_n=10)
    for combo, dec, tp, ev in results:
        legs = " | ".join([f"{c.market_id}:{c.outcome}@{c.best_book} ({c.best_american})" for c in combo])
        print(f"EV={ev:.3f}, TrueP={tp:.4f}, OfferedDec={dec:.3f} :: {legs}")