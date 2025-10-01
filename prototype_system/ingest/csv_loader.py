# This is a simple program that loads quotes from a CSV file.

from typing import List
from analytics.line_compare import BookQuote
import csv

# expected columns: book, market_id, outcome, american

def load_quotes_csv(path: str) -> List[BookQuote]:
    out: List[BookQuote] = []
    with open(path, newline='') as f:
        reader = csv.DictReader(f)
        for row in reader:
            out.append(BookQuote(
                book=row['book'],
                market_id=row['market_id'],
                outcome=row['outcome'],
                american=int(row['american'])
            ))
    return out