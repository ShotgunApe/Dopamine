<h2 align=center> Dopamine </h2>

<p align=center> WIP R5900 Disassembler for Linux/PS Vita </p>

![A Screenshot of the application.](https://i.imgur.com/hAIwTBI.png)

### Building
These instructions assume you are using a Debian-based distribution. Package names may vary slightly for other distributions.

1. `sudo apt-get install make git openssh-client bzip2 wget curl xz-utils ninja-build cmake python3 libgl1-mesa-dev libglu1-mesa-dev`
2. Install [Vita SDK](https://vitasdk.org/) and add it to your `$PATH` (optional if not building for PS Vita)
3. `git clone --recursive git@github.com:ShotgunApe/Dopamine.git`
4. `cd Dopamine`
5. `mkdir build && cd build`
6. `cmake .. -DTARGET_PLATFORM=vita && make` or open `CMakeLists.txt` in your favorite IDE

If building for Linux, set `-DTARGET_PLATFORM=linux`.

If you run into issues cloning submodules, ensure you have a working SSH key with GitHub, or add the dependencies manually to `./lib`.

### Running
Dopamine requires that `libshacccg.suprx` is extracted and decrypted on your console. This can be done using [ShaRKF00D](https://github.com/OsirizX/ShaRKF00D/releases/tag/1.3).

1. Transfer `Dopamine.vpk` to your console and install it with VitaShell
2. Run the program

### Resources
- Vita SDK [Samples](https://github.com/vitasdk/samples)
- DobieStation [docs](https://github.com/PSI-Rockin/DobieStation/wiki) and [ps2tek](https://psi-rockin.github.io/ps2tek/) for PS2 info
- [Vita3K](https://github.com/Vita3K/Vita3K/) for initial testing
- [PCSX2 Debugger](https://github.com/PCSX2/pcsx2)
- [imgui-vita](https://github.com/Rinnegatamante/imgui-vita) for GUI
- [doctest](https://github.com/doctest/doctest) for being a sick testing library

