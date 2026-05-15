//
// Created by Aether on 15.05.2026.
//

#pragma once

// Engine::Path / Engine::PathView — platform-native path types.
//
// On Windows  → Path = WString,   PathView = WStringView  (UTF-16, zero-cost Win32 interop)
// On Linux    → Path = String,    PathView = StringView   (UTF-8)
// On macOS    → Path = String,    PathView = StringView   (UTF-8)
//
// AETHER_TEXT(s) produces the correct literal form for each platform:
//   AETHER_TEXT("assets/foo.png")  →  L"assets/foo.png"  on Windows
//                                  →   "assets/foo.png"  elsewhere
//
// For conversions between encodings include <Aether/Core/Unicode.h>:
//   PathFromUtf8(StringView)   — construct a Path from a UTF-8 string
//   PathToUtf8(PathView)       — extract a UTF-8 String from any Path
//
// Including this header does NOT pull in the full template body.  Code that only
// mentions Path/PathView in function signatures can stay cheap by including this
// header alone and <Aether/Core/String.h> / <Aether/Core/WString.h> in the .cpp.

#include <Aether/Core/Core.h>
#include <Aether/Core/String.h>
#include <Aether/Core/StringView.h>

namespace Aether::Engine {

#if defined(AETHER_PLATFORM_WINDOWS)
    using PathChar = wchar_t;
    using Path     = WString;
    using PathView = WStringView;
    #define AETHER_TEXT(s) L##s
#else
    using PathChar = char;
    using Path = String;
    using PathView = StringView;
    #define AETHER_TEXT(s) s
#endif

} // namespace Aether::Engine
