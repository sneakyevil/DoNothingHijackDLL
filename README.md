# DoNothingHijackDLL
Hijacks DLL of process &amp; create thread and attempts to suspend all running threads so the process doesn't use any CPU resources.

# Compiling Requirements
The dllmain.cpp includes file from: [DLL-Hijack/Microsoft/VersionHijack.hpp](https://github.com/sneakyevil/DLL-Hijack/blob/main/Microsoft/VersionHijack.hpp)

# Point of repo
Since certain version of Windows 10 there is small bloat application related to Snipping Tool that are running constantly in background called `TextInputHost.exe`, simply removing/renaming the application will cause DCOM Error spam. Placing hijacked dll of this project inside the folder will stop the process from using any cpu resources.
