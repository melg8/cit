window.BENCHMARK_DATA = {
  "lastUpdate": 1673939154197,
  "repoUrl": "https://github.com/melg8/cit",
  "entries": {
    "Catch2 Benchmark clang++": [
      {
        "commit": {
          "author": {
            "email": "public.melg8@gmail.com",
            "name": "Melg Eight",
            "username": "melg8"
          },
          "committer": {
            "email": "public.melg8@gmail.com",
            "name": "Melg Eight",
            "username": "melg8"
          },
          "distinct": true,
          "id": "e6f491a92b48f93522ad3ab7ea8102c6b5670adc",
          "message": "test: move usability tests to benchmark\n\nMove usability tests to benchmark.\n\nSigned-off-by: Melg Eight <public.melg8@gmail.com>",
          "timestamp": "2023-01-17T09:56:06+03:00",
          "tree_id": "9eeae98b08fe080bd0998ce1a51cf2d286720f8e",
          "url": "https://github.com/melg8/cit/commit/e6f491a92b48f93522ad3ab7ea8102c6b5670adc"
        },
        "date": 1673939153649,
        "tool": "catch2",
        "benches": [
          {
            "name": "compare raw pointers",
            "value": 2.43949,
            "range": "+/- 219.866",
            "unit": "us",
            "extra": "100 samples\n22 iterations"
          },
          {
            "name": "compare smart pointers",
            "value": 2.58814,
            "range": "+/- 309.009",
            "unit": "us",
            "extra": "100 samples\n21 iterations"
          },
          {
            "name": "compare modernized",
            "value": 3.56766,
            "range": "+/- 293.487",
            "unit": "us",
            "extra": "100 samples\n15 iterations"
          }
        ]
      }
    ]
  }
}