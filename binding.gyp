{
  'targets': [
    {
      "variables": {
        "dht_verbose%": "false"
      },
      "target_name": "node-ad-da",
      "sources": [ "node-ad-da.cpp", "ad-da.cpp" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "libraries": [ "-lbcm2835" ],
      "conditions": [
        ["OS=='linux'", {
          "cflags": [ "-std=c++11" ],
          "include_dirs+": "/usr/local/lib/libbcm2835.a",
          "sources": ["node-ad-da.cpp", "ad-da.cpp" ]
        }],
        ["dht_verbose=='true'", {
          "defines": [ "VERBOSE" ]
        }]
      ]
    }
  ]
}
