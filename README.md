slim
====

"slim" is a minimal, cross-platform (Linux, Mac OS X, Windows) set of files comprised of common functionality. It is written to C99, but not very strictly. The goal of this "library", is to provide drop-in cross-platform code to easily add functionality listed in one of the provided modules.

It includes the following "modules":

- xlib - Dynamic / Shared Library handling
- xlog - Simple logging functionality; supports multiple "log handlers"
- xnet - BSD-socket like interface across platforms
- xstr - Compact ANSI C-string interface
- xthread - Basic support for threads and mutexes; probably needs additional work
- xtime - Simplified interface to high-performance timers



## Roadmap
- Doxygen comments
- Remove bloat / cleanup
- Bug fixes
