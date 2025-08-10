<h2 align=center> Dopamine </h2>

<p align=center> Experimental PS2 Emulator for PS Vita </p>

### Getting Started
1. `sudo apt install make git-core cmake python` (cmake >= 3.16 required)
2. install [Vita SDK](https://vitasdk.org/) and add it to your `$PATH`
3. `git clone --recursive git@github.com:ShotgunApe/Dopamine.git`
4. `mkdir build && cd build`
5. `cmake .. && make` or open `CMakeLists.txt` in your favorite IDE

### Resources
- Vita SDK [Samples](https://github.com/vitasdk/samples) for text rendering
- [PCSX2](https://github.com/PCSX2/pcsx2) for CPU structs
- DobieStation [docs](https://github.com/PSI-Rockin/DobieStation/wiki) for PS2 emulation
- [Vita3K](https://github.com/Vita3K/Vita3K/) for testing
