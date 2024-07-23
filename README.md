# Computer architectures and operating systems - Group project

This is the Group project that I have done with some of my colleagues at Politecnico di Torino for the [Computer architectures and operating systems Exam](https://didattica.polito.it/pls/portal30/gap.pkg_guide.viewGap?p_cod_ins=01GYKUV&p_a_acc=2024&p_header=S&p_lang=&multi=N).

## Description

This project aims to explore the usage of a real-time operating system, with hand-practice. The used operating system was FreeRTOS, in which we implemented some demos and added a new feature. 

## Structure of the Repository
- **Documentation**
  - _HaclOSsim - Group09.pptx_
  - _HaclOSsim - Group09.pdf_
  - _Report.pdf_
  - _Report.tex_
- **Examples**
  - _demoTimer.c_
  - _demoSemaphores.c_
  - _demoStats.c_
  - _demoHospital.c_
  - _demoHospital2.c_
  - _demoMatrix.c_
  - **DemoScheduler**
    - _demoScheduler.c_
- **FreeRTOS**
  
## Installation and usage of the repository
- ## QEMU Installation<br>
  - ### Windows Users<br>
    Go to https://www.qemu.org/download/ and search for the appropriate installer based on your operating system. Once you choose the appropriate section, choose the appropriate processor architecture (32/64-bit) and you will be able to find the last version of the executable file (_`qemu-w64-setup-x.exe`_, where “x” is the last release date).<br> After installing the .exe file, execute it and follow the passages to install Qemu.<br> Pay attention to the path of the folder where you are installing the emulator, to start it easily.
  - ### MacOS Users<br>
    QEMU can be installed from Homebrew:<br>
    _`brew install qemu`_<br>
    QEMU can be installed from MacPorts:<br>
    _`sudo port install qemu`_<br>
    QEMU requires the most recent macOS version that is currently available. Support for the last previous version will be dropped two years after the current version has been released or when Apple drops support for it, whatever comes first.
  - ### Linux Users<br>
    QEMU is packaged by most Linux distributions:
      - Arch: _`pacman -S qemu`_
      - Debian/Ubuntu: 
        - For full system emulation: _`apt-get install qemu-system`_<br>
        - For emulating Linux binaries: _`apt-get install qemu-user-static`_<br>
      - Fedora: _`dnf install @virtualization`_
      - Gentoo: _`emerge --ask app-emulation/qemu`_
      - RHEL/CentOS: _`yum install qemu-kvm`_
      - SUSE: _`zypper install qemu`_
- ## FreeRTOS Installation<br>
  Go to https://www.freertos.org/a00104.html. Download from the "Download" button and extract the content.<br>
  Package contains the FreeRTOS Kernel, FreeRTOS-Plus libraries and AWS IoT libraries, along with example projects.
- ## GNU Make Installation
  Go to https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads.<br>
  - ### Windows Users<br>
    Find the release called “AArch32 bare-metal target (arm-none-eabi)” and download the .exe file provided by the web site.
  - ### MacOS Users<br>
    Find the release called “AArch32 bare-metal target (arm-none-eabi)” and download the .pkg file provided by the web site.
  - ### Debian-based Users<br>
    Install ARM GNU Toolchain using the following terminal commands:<br>
    _`sudo apt install gcc-arm-none-eabi`_<br>
    _`sudo apt install gdb-arm-none-eabi`_<br>
    _`sudo apt install binutils-arm-none-eabi`_<br>
- ## How to compile FreeRTOS
  To make your work easier, we recommend downloading and install Visual Studio Code.
  - ### Windows Users
    Make sure that ARM GNU Toolchain and GNU Make are in environmental variables path, otherwise, you can add it manually copying the path of the tool and pasting it in this location `Start > Edit System Variable > Path`.
    Subsequently, you will be able to run, with Visual Studio Code, the file main.c which is reachable in this path `FreeRTOSv202212.01\FreeRTOS\ \<Folder Name\> \CORTEX_MPS2_QEMU_IAR_GCC\` and, if you followed carefully all the steps, you will find a .out file called `RTOSDemo.out` with this path `FreeRTOSv202212.01\FreeRTOS\ \<Folder Name\> \CORTEX_MPS2_QEMU_IAR_GCC\build\gcc\output\`.<br>
    Note that `\<Folder Name\>` in the filesystem is one of the three folders `Examples`, `Examples\DemoScheduler`, or `FreeRTOS`.
  - ### MacOS Users
    Go to `FreeRTOSv202212.01/FreeRTOS/\<Folder Name\>/CORTEX_MPS2_QEMU_IAR_GCC/build/gcc` and from the terminal digit the command _`make`_. When the running will finish and the system will return the terminal, you will be able to run the demo.
    Note that `\<Folder Name\>` in the filesystem is one of the three folders `Examples`, `Examples/DemoScheduler`, or `FreeRTOS`.
  - ### Linux Users
    The actions to perform are the same as in the [MacOS Users section](#macos-users-2).
