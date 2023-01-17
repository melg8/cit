window.BENCHMARK_DATA = {
  "lastUpdate": 1673950467663,
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
          "id": "626840f64695e7f77f577265ecc1049cb0eaa022",
          "message": "test: add more isolated benchmarks\n\nAdd more isolated benchmarks.\n\nSigned-off-by: Melg Eight <public.melg8@gmail.com>",
          "timestamp": "2023-01-17T12:26:47+03:00",
          "tree_id": "656ef51237687f9ae49223cfcd0b4551459aaa7e",
          "url": "https://github.com/melg8/cit/commit/626840f64695e7f77f577265ecc1049cb0eaa022"
        },
        "date": 1673948059184,
        "tool": "catch2",
        "benches": [
          {
            "name": "creation with value",
            "value": 1.51475,
            "range": "+/- 269.408",
            "unit": "us",
            "extra": "100 samples\n40 iterations"
          },
          {
            "name": "compare raw pointers",
            "value": 2.73228,
            "range": "+/- 544.939",
            "unit": "us",
            "extra": "100 samples\n22 iterations"
          },
          {
            "name": "smart pointer with value",
            "value": 1.44126,
            "range": "+/- 257.245",
            "unit": "us",
            "extra": "100 samples\n43 iterations"
          },
          {
            "name": "compare smart pointers",
            "value": 2.63066,
            "range": "+/- 545.306",
            "unit": "us",
            "extra": "100 samples\n23 iterations"
          },
          {
            "name": "modernized with value",
            "value": 2.0179,
            "range": "+/- 967.104",
            "unit": "us",
            "extra": "100 samples\n30 iterations"
          },
          {
            "name": "compare modernized",
            "value": 4.35467,
            "range": "+/- 1.47676",
            "unit": "us",
            "extra": "100 samples\n15 iterations"
          },
          {
            "name": "modernized with value of",
            "value": 4.29307,
            "range": "+/- 963.723",
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
          "id": "2c0a6606ab5903ae82c674e294196c94dd469299",
          "message": "chore: remove statics\n\nRemove static.\n\nSigned-off-by: Melg Eight <public.melg8@gmail.com>",
          "timestamp": "2023-01-17T13:06:13+03:00",
          "tree_id": "158ac297f51b6d1b923cbf54c83a79da87cb2c87",
          "url": "https://github.com/melg8/cit/commit/2c0a6606ab5903ae82c674e294196c94dd469299"
        },
        "date": 1673950422754,
        "tool": "catch2",
        "benches": [
          {
            "name": "creation with value",
            "value": 1.25494,
            "range": "+/- 136.92",
            "unit": "us",
            "extra": "100 samples\n38 iterations"
          },
          {
            "name": "compare raw pointers",
            "value": 2.2679,
            "range": "+/- 240.054",
            "unit": "us",
            "extra": "100 samples\n21 iterations"
          },
          {
            "name": "smart pointer with value",
            "value": 1.21492,
            "range": "+/- 117.811",
            "unit": "us",
            "extra": "100 samples\n40 iterations"
          },
          {
            "name": "compare smart pointers",
            "value": 2.29609,
            "range": "+/- 154.391",
            "unit": "us",
            "extra": "100 samples\n21 iterations"
          },
          {
            "name": "modernized with value",
            "value": 1.72588,
            "range": "+/- 87.7577",
            "unit": "us",
            "extra": "100 samples\n27 iterations"
          },
          {
            "name": "compare modernized",
            "value": 3.51076,
            "range": "+/- 310.98",
            "unit": "us",
            "extra": "100 samples\n13 iterations"
          },
          {
            "name": "modernized with value of",
            "value": 3.62468,
            "range": "+/- 262.767",
            "unit": "us",
            "extra": "100 samples\n13 iterations"
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
          "id": "626840f64695e7f77f577265ecc1049cb0eaa022",
          "message": "test: add more isolated benchmarks\n\nAdd more isolated benchmarks.\n\nSigned-off-by: Melg Eight <public.melg8@gmail.com>",
          "timestamp": "2023-01-17T12:26:47+03:00",
          "tree_id": "656ef51237687f9ae49223cfcd0b4551459aaa7e",
          "url": "https://github.com/melg8/cit/commit/626840f64695e7f77f577265ecc1049cb0eaa022"
        },
        "date": 1673948006702,
        "tool": "catch2",
        "benches": [
          {
            "name": "creation with value",
            "value": 1.24185,
            "range": "+/- 147.705",
            "unit": "us",
            "extra": "100 samples\n48 iterations"
          },
          {
            "name": "compare raw pointers",
            "value": 2.28496,
            "range": "+/- 212.878",
            "unit": "us",
            "extra": "100 samples\n26 iterations"
          },
          {
            "name": "smart pointer with value",
            "value": 1.1856,
            "range": "+/- 106.606",
            "unit": "us",
            "extra": "100 samples\n50 iterations"
          },
          {
            "name": "compare smart pointers",
            "value": 2.50634,
            "range": "+/- 203.729",
            "unit": "us",
            "extra": "100 samples\n23 iterations"
          },
          {
            "name": "modernized with value",
            "value": 1.71397,
            "range": "+/- 43.1944",
            "unit": "us",
            "extra": "100 samples\n35 iterations"
          },
          {
            "name": "compare modernized",
            "value": 3.57281,
            "range": "+/- 317.282",
            "unit": "us",
            "extra": "100 samples\n16 iterations"
          },
          {
            "name": "modernized with value of",
            "value": 3.62418,
            "range": "+/- 213.521",
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
          "id": "2c0a6606ab5903ae82c674e294196c94dd469299",
          "message": "chore: remove statics\n\nRemove static.\n\nSigned-off-by: Melg Eight <public.melg8@gmail.com>",
          "timestamp": "2023-01-17T13:06:13+03:00",
          "tree_id": "158ac297f51b6d1b923cbf54c83a79da87cb2c87",
          "url": "https://github.com/melg8/cit/commit/2c0a6606ab5903ae82c674e294196c94dd469299"
        },
        "date": 1673950467230,
        "tool": "catch2",
        "benches": [
          {
            "name": "creation with value",
            "value": 1.23174,
            "range": "+/- 125.039",
            "unit": "us",
            "extra": "100 samples\n48 iterations"
          },
          {
            "name": "compare raw pointers",
            "value": 2.24091,
            "range": "+/- 201.65",
            "unit": "us",
            "extra": "100 samples\n27 iterations"
          },
          {
            "name": "smart pointer with value",
            "value": 1.15742,
            "range": "+/- 72.9212",
            "unit": "us",
            "extra": "100 samples\n51 iterations"
          },
          {
            "name": "compare smart pointers",
            "value": 2.1308,
            "range": "+/- 267.035",
            "unit": "us",
            "extra": "100 samples\n27 iterations"
          },
          {
            "name": "modernized with value",
            "value": 1.71433,
            "range": "+/- 60.3592",
            "unit": "us",
            "extra": "100 samples\n35 iterations"
          },
          {
            "name": "compare modernized",
            "value": 3.57741,
            "range": "+/- 293.979",
            "unit": "us",
            "extra": "100 samples\n16 iterations"
          },
          {
            "name": "modernized with value of",
            "value": 3.69766,
            "range": "+/- 341.371",
            "unit": "us",
            "extra": "100 samples\n16 iterations"
          }
        ]
      }
    ]
  }
}