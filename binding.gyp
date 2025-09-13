{
  "variables": {
    "addon_name": "remote_input"
  },
  "targets": [
    {
      "target_name": "<(addon_name)",
      "sources": [
        "RemoteInput/Plugin/Node/NodeAddon.cxx",
        "RemoteInput/Plugin/Node/NodeEIOS.cxx"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")>",
        "<(module_root_dir)/node_modules/node-addon-api",
        "RemoteInput"
      ],
      "defines": [
        "NAPI_VERSION=8",
        "WIN32_LEAN_AND_MEAN",
        "NOMINMAX"
      ],
      "cflags_cc": [
        "-std=c++20"
      ],
      "conditions": [
        ["OS=='win'", {
          "msvs_settings": {
            "VCCLCompilerTool": {
              "AdditionalOptions": ["/std:c++20", "/EHsc"]
            }
          }
        }],
        ["OS=='mac'", {
          "xcode_settings": {
            "CLANG_CXX_LANGUAGE_STANDARD": "c++20",
            "CLANG_CXX_LIBRARY": "libc++"
          }
        }]
      ]
    }
  ]
}