{
  "variables": {
    "addon_name": "remote_input"
  },
  "targets": [
    {
      "target_name": "<(addon_name)",
      "win_delay_load_hook": "false",
      "sources": [
        "RemoteInput/Plugin/Node/NodeAddon.cxx",
        "RemoteInput/Plugin/Node/NodeEIOS.cxx",
        "RemoteInput/Thirdparty/min_hook/src/hook.c",
        "RemoteInput/Thirdparty/min_hook/src/trampoline.c",
        "RemoteInput/Thirdparty/min_hook/src/buffer.c",
        "RemoteInput/Thirdparty/min_hook/src/hde/hde64.c",
        "RemoteInput/RemoteInput.h",
        "RemoteInput/Echo/Atomics.hxx",
        "RemoteInput/Echo/Mutex.cxx",
        "RemoteInput/Echo/Mutex.hxx",
        "RemoteInput/Echo/Semaphore.cxx",
        "RemoteInput/Echo/Semaphore.hxx",
        "RemoteInput/Echo/Event.cxx",
        "RemoteInput/Echo/Event.hxx",
        "RemoteInput/Echo/Synchronization.hxx",
        "RemoteInput/Echo/MemoryMap.cxx",
        "RemoteInput/Echo/MemoryMap.hxx",
        "RemoteInput/Echo/MemoryMapStream.hxx",
        "RemoteInput/Echo/Module.cxx",
        "RemoteInput/Echo/Module.hxx",
        "RemoteInput/Echo/Stream.cxx",
        "RemoteInput/Echo/Stream.hxx",
        "RemoteInput/Echo/Time.cxx",
        "RemoteInput/Echo/Time.hxx",
        "RemoteInput/Echo/TypeTraits.hxx",
        "RemoteInput/Echo/TypeTraits_Functional.hxx",
        "RemoteInput/Echo/TypeTraits_Functional_Attributes.hxx",
        "RemoteInput/Java/JNI_Common.hxx",
        "RemoteInput/Java/Applet.cxx",
        "RemoteInput/Java/Applet.hxx",
        "RemoteInput/Java/AWTAccessor.cxx",
        "RemoteInput/Java/AWTAccessor.hxx",
        "RemoteInput/Java/AWTEvent.cxx",
        "RemoteInput/Java/AWTEvent.hxx",
        "RemoteInput/Java/AWTEventAccessor.cxx",
        "RemoteInput/Java/AWTEventAccessor.hxx",
        "RemoteInput/Java/Component.cxx",
        "RemoteInput/Java/Component.hxx",
        "RemoteInput/Java/Container.cxx",
        "RemoteInput/Java/Container.hxx",
        "RemoteInput/Java/EventQueue.cxx",
        "RemoteInput/Java/EventQueue.hxx",
        "RemoteInput/Java/FocusEvent.cxx",
        "RemoteInput/Java/FocusEvent.hxx",
        "RemoteInput/Java/Frame.cxx",
        "RemoteInput/Java/Frame.hxx",
        "RemoteInput/Java/InputEvent.cxx",
        "RemoteInput/Java/InputEvent.hxx",
        "RemoteInput/Java/KeyEvent.cxx",
        "RemoteInput/Java/KeyEvent.hxx",
        "RemoteInput/Java/MouseEvent.cxx",
        "RemoteInput/Java/MouseEvent.hxx",
        "RemoteInput/Java/MouseWheelEvent.cxx",
        "RemoteInput/Java/MouseWheelEvent.hxx",
        "RemoteInput/Java/PointerInfo.cxx",
        "RemoteInput/Java/PointerInfo.hxx",
        "RemoteInput/Java/SunToolkit.cxx",
        "RemoteInput/Java/SunToolkit.hxx",
        "RemoteInput/Java/Toolkit.cxx",
        "RemoteInput/Java/Toolkit.hxx",
        "RemoteInput/Java/Window.cxx",
        "RemoteInput/Java/Window.hxx",
        "RemoteInput/Java/WindowEvent.cxx",
        "RemoteInput/Java/WindowEvent.hxx",
        "RemoteInput/Java/RIEventQueue.cxx",
        "RemoteInput/Java/RIEventQueue.hxx",
        "RemoteInput/Platform/DebugConsole.cxx",
        "RemoteInput/Platform/DebugConsole.hxx",
        "RemoteInput/Platform/JavaInternal.hxx",
        "RemoteInput/Platform/NativeHooks.hxx",
        "RemoteInput/Platform/Platform.hxx",
        "RemoteInput/Plugin/ControlCenter.cxx",
        "RemoteInput/Plugin/ControlCenter.hxx",
        "RemoteInput/Plugin/Graphics.cxx",
        "RemoteInput/Plugin/Graphics.hxx",
        "RemoteInput/Plugin/ImageData.cxx",
        "RemoteInput/Plugin/ImageData.hxx",
        "RemoteInput/Plugin/InputOutput.cxx",
        "RemoteInput/Plugin/InputOutput.hxx",
        "RemoteInput/Plugin/NativePlugin.cxx",
        "RemoteInput/Plugin/NativePlugin.hxx",
        "RemoteInput/Plugin/Plugin.cxx",
        "RemoteInput/Plugin/Plugin.hxx",
        "RemoteInput/Plugin/Signal.hxx",
        "RemoteInput/Plugin/TMemoryManager.hxx",
        "RemoteInput/Plugin/JVM/JVMCache.cxx",
        "RemoteInput/Plugin/JVM/JVMCache.hxx",
        "RemoteInput/Plugin/JVM/RemoteVM.hxx",
        "RemoteInput/Plugin/JVM/RemoteVM.cxx",
        "RemoteInput/DetachedThreadPool.cxx",
        "RemoteInput/DetachedThreadPool.hxx",
        "RemoteInput/EIOS.cxx",
        "RemoteInput/EIOS.hxx",
        "RemoteInput/JVM.cxx",
        "RemoteInput/JVM.hxx",
        "RemoteInput/Random.cxx",
        "RemoteInput/Random.hxx",
        "RemoteInput/Reflection.cxx",
        "RemoteInput/Reflection.hxx",
        "RemoteInput/ReflectionHook.cxx",
        "RemoteInput/ReflectionHook.hxx",
        "RemoteInput/RemoteInput.h",
        "RemoteInput/ThreadPool.cxx",
        "RemoteInput/ThreadPool.hxx",
        "RemoteInput/Injection/Injector.hxx",
        "RemoteInput/Thirdparty/Injector.cxx",
        "RemoteInput/Thirdparty/Hook.cxx"
        # "RemoteInput/EIOS.cxx",
        # "RemoteInput/Random.cxx",
        # "RemoteInput/Plugin/ControlCenter.cxx",
        # "RemoteInput/Plugin/ImageData.cxx",
        # "RemoteInput/Plugin/NativePlugin.cxx",
        # "RemoteInput/Plugin/InputOutput.cxx",
        # "RemoteInput/Plugin/JVM/RemoteVM.cxx",
        # "RemoteInput/Reflection.cxx",
        # "RemoteInput/ReflectionHook.cxx",
        # "RemoteInput/JVM.cxx",
        # "RemoteInput/Echo/Semaphore.cxx",
        # "RemoteInput/Echo/Event.cxx",
        # "RemoteInput/Echo/Stream.cxx",
        # "RemoteInput/Echo/MemoryMap.cxx",
        # "RemoteInput/Plugin/JVM/JVMCache.cxx",
        # "RemoteInput/Thirdparty/Injector.cxx",
        # "RemoteInput/Plugin/Node/NodeAddon.cxx",
        # "RemoteInput/Plugin/Node/NodeEIOS.cxx",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")>",
        "<(module_root_dir)/node_modules/node-addon-api",
        "RemoteInput",
        "RemoteInput/Plugin",
        "RemoteInput/Plugin/JVM",
        "RemoteInput/Echo",
        "RemoteInput/Java",
        "RemoteInput/Platform",
        "RemoteInput/Injection",
        "RemoteInput/Thirdparty/kubo_injector/src",
        "RemoteInput/Thirdparty/kubo_injector/include",
        "RemoteInput/Thirdparty/min_hook/include",
        "RemoteInput/Thirdparty/min_hook/src"
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
          "libraries": [
            "user32.lib",
            "opengl32.lib",
            "gdi32.lib",
            "dbghelp.lib"
          ],
          "msvs_settings": {
            "VCCLCompilerTool": {
              "AdditionalOptions": ["/std:c++20", "/EHsc"]
            }
          },
          "sources": [
            "RemoteInput/Thirdparty/kubo_injector/src/windows/injector.c",
            "RemoteInput/Platform/NativeHooks_Windows.cxx",
            "RemoteInput/Injection/Injector_Windows.cxx",
            "RemoteInput/Platform/Platform_Windows.cxx"
          ],
          "include_dirs": [
            "C:/Program Files/Java/jdk-21/include",
            "C:/Program Files/Java/jdk-21/include/win32"
          ]
        }],
        ["OS=='mac'", {
          "xcode_settings": {
            "CLANG_CXX_LANGUAGE_STANDARD": "c++20",
            "CLANG_CXX_LIBRARY": "libc++"
          },
          "libraries": [
            "-framework Foundation",
            "-framework Cocoa",
            "-framework OpenGL",
            "-ldl",
            "-lpthread"
          ],
          "include_dirs": [
            "/Library/Java/JavaVirtualMachines/jdk-21.jdk/Contents/Home/include",
            "/Library/Java/JavaVirtualMachines/jdk-21.jdk/Contents/Home/include/darwin"
          ],
          "sources": [
            "RemoteInput/Thirdparty/kubo_injector/src/linux/injector.c",
            "RemoteInput/Platform/NativeHooks_Darwin.cxx",
            "RemoteInput/Platform/Platform_Darwin.mm",
            "RemoteInput/Injection/Injector_Darwin.cxx",
            "RemoteInput/Injection/Injector_Arm.cpp"
          ]
        }],
        ["OS=='linux'", {
          "libraries": [
            "-ldl",
            "-lpthread",
            "-lGL",
            "-lrt",
            "-lX11"
          ],
          "include_dirs": [
            "/usr/lib/jvm/java-21-openjdk/include",
            "/usr/lib/jvm/java-21-openjdk/include/linux"
          ],
          "sources": [
            "RemoteInput/Thirdparty/kubo_injector/src/linux/injector.c",
            "RemoteInput/Platform/NativeHooks_Linux.cxx",
            "RemoteInput/Platform/Platform_Linux.cxx",
            "RemoteInput/Injection/Injector_Linux.cpp",
          ]
        }]
      ]
    }
  ]
}