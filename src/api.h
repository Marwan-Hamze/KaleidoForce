#pragma once

#if defined _WIN32 || defined __CYGWIN__
#  define KaleidoForce_DLLIMPORT __declspec(dllimport)
#  define KaleidoForce_DLLEXPORT __declspec(dllexport)
#  define KaleidoForce_DLLLOCAL
#else
// On Linux, for GCC >= 4, tag symbols using GCC extension.
#  if __GNUC__ >= 4
#    define KaleidoForce_DLLIMPORT __attribute__((visibility("default")))
#    define KaleidoForce_DLLEXPORT __attribute__((visibility("default")))
#    define KaleidoForce_DLLLOCAL __attribute__((visibility("hidden")))
#  else
// Otherwise (GCC < 4 or another compiler is used), export everything.
#    define KaleidoForce_DLLIMPORT
#    define KaleidoForce_DLLEXPORT
#    define KaleidoForce_DLLLOCAL
#  endif // __GNUC__ >= 4
#endif // defined _WIN32 || defined __CYGWIN__

#ifdef KaleidoForce_STATIC
// If one is using the library statically, get rid of
// extra information.
#  define KaleidoForce_DLLAPI
#  define KaleidoForce_LOCAL
#else
// Depending on whether one is building or using the
// library define DLLAPI to import or export.
#  ifdef KaleidoForce_EXPORTS
#    define KaleidoForce_DLLAPI KaleidoForce_DLLEXPORT
#  else
#    define KaleidoForce_DLLAPI KaleidoForce_DLLIMPORT
#  endif // KaleidoForce_EXPORTS
#  define KaleidoForce_LOCAL KaleidoForce_DLLLOCAL
#endif // KaleidoForce_STATIC