window.BENCHMARK_DATA = {
  "lastUpdate": 1673944856618,
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
      },
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
          "id": "db6d88fb46af57e6c21016e2d38cd24d82575227",
          "message": "chore: add benchmark for usage of value comparison\n\nAdd benchmark for usage of value comparison.\n\nSigned-off-by: Melg Eight <public.melg8@gmail.com>",
          "timestamp": "2023-01-17T11:30:06+03:00",
          "tree_id": "a86fe304bd994c752e9284054d2ee9a2e6f19248",
          "url": "https://github.com/melg8/cit/commit/db6d88fb46af57e6c21016e2d38cd24d82575227"
        },
        "date": 1673944855942,
        "tool": "catch2",
        "benches": [
          {
            "name": "compare raw pointers",
            "value": 2.90692,
            "range": "+/- 833.338",
            "unit": "us",
            "extra": "100 samples\n22 iterations"
          },
          {
            "name": "compare smart pointers",
            "value": 2.93991,
            "range": "+/- 1.13467",
            "unit": "us",
            "extra": "100 samples\n21 iterations"
          },
          {
            "name": "compare modernized",
            "value": 4.251,
            "range": "+/- 1.31295",
            "unit": "us",
            "extra": "100 samples\n14 iterations"
          },
          {
            "name": "modernized with value of",
            "value": 4.7712,
            "range": "+/- 1.87791",
            "unit": "us",
            "extra": "100 samples\n15 iterations"
          }
        ]
      }
    ],
    "Catch2 Benchmark g++": [
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
        "date": 1673939210214,
        "tool": "catch2",
        "benches": [
          {
            "name": "compare raw pointers",
            "value": 2.25284,
            "range": "+/- 191.422",
            "unit": "us",
            "extra": "100 samples\n26 iterations"
          },
          {
            "name": "compare smart pointers",
            "value": 2.43567,
            "range": "+/- 226.373",
            "unit": "us",
            "extra": "100 samples\n25 iterations"
          },
          {
            "name": "compare modernized",
            "value": 3.66461,
            "range": "+/- 321.324",
            "unit": "us",
            "extra": "100 samples\n16 iterations"
          }
        ]
      },
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
          "id": "db6d88fb46af57e6c21016e2d38cd24d82575227",
          "message": "chore: add benchmark for usage of value comparison\n\nAdd benchmark for usage of value comparison.\n\nSigned-off-by: Melg Eight <public.melg8@gmail.com>",
          "timestamp": "2023-01-17T11:30:06+03:00",
          "tree_id": "a86fe304bd994c752e9284054d2ee9a2e6f19248",
          "url": "https://github.com/melg8/cit/commit/db6d88fb46af57e6c21016e2d38cd24d82575227"
        },
        "date": 1673944797546,
        "tool": "catch2",
        "benches": [
          {
            "name": "compare raw pointers",
            "value": 2.27171,
            "range": "+/- 200.077",
            "unit": "us",
            "extra": "100 samples\n26 iterations"
          },
          {
            "name": "compare smart pointers",
            "value": 2.43568,
            "range": "+/- 197.925",
            "unit": "us",
            "extra": "100 samples\n24 iterations"
          },
          {
            "name": "compare modernized",
            "value": 3.60016,
            "range": "+/- 236.508",
            "unit": "us",
            "extra": "100 samples\n16 iterations"
          },
          {
            "name": "modernized with value of",
            "value": 3.72685,
            "range": "+/- 225.579",
            "unit": "us",
            "extra": "100 samples\n16 iterations"
          }
        ]
      }
    ]
  }
}