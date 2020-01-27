# PineTime Hermes - Firmware
Firmware for the smart watch [PineTime](https://wiki.pine64.org/index.php/PineTime) from [PINE64](https://www.pine64.org/).
Primarily focused on displaying time and notifications.

Built using [Zephyr Project](https://zephyrproject.org/).

 Home Screen | Console Screen
------------ | -------------
![Home Screen](screenshots/img_home.jpg) | ![Console Screen](screenshots/img_console.jpg)

> **Hermes**, messenger of the gods.
> He can convey messages between the divine realms, the underworld, and the world of mortals.

# Getting Started
## Install Zephyr Dependencies
Follow the official steps: https://docs.zephyrproject.org/latest/getting_started/index.html

*Note:* use this repository's directory whenever you see mention of '~/zephyrproject'

1. OS Update
2. Install Dependencies
3. Get the Source Code (don't create a new project, clone this repository!)
     ```
     cd ~/
     git clone --recurse-submodules https://github.com/Dejvino/pinetime-hermes-firmware.git
     cd pinetime-hermes-firmware
     west update
     ```
4. Install Needed Python Packages
5. Install Software Development Toolchain
 
## Build the App
  ```
  cd ~/pinetime-hermes-firmware
  west build -p -b pinetime ./app
  ```
  
## Flash the App
  ```
  west flash
  ```
  or
  ```
  ./jlink-flash.sh
  ```
  
Enjoy!

# Current Status
* **clock**
  * renders green digital clock with low precision time updates.
* **power management**
  * backlight is enabled by holding the button, remains turned on for a few seconds.
  * mocked battery level is available
* **windows**
  * basic support for different windows (AKA screens/views/apps) shown on the display
    * **home** shows a big clock, Bluetooth status, Battery level and a single notification message
    * **console** shows a clock and a large scrolling console with several notification messages
  * support for switching between the windows (not tested or used)
* **notifications**
  * local in-memory storage of last X notifications
* **Bluetooth**
  * connection status is indicated on the display
  * advertises mock Battery level, Heart-rate sensor data, Current Time Service (CTS).
  * synchronizes its clock from a connected device (if it provides CTS service).
  * accepts notifications pushed to the device

Note: for the notifications to work you need a compatible Companion App, see below.

# Related Links
* Hermes Companion Apps: https://github.com/Dejvino/pinetime-hermes-companion
   * Linux / PinePhone: [companion-linux](https://github.com/Dejvino/pinetime-hermes-companion-linux)
   * Android: [companion-android](https://github.com/Dejvino/pinetime-hermes-companion-android)
* PineTime Zephyr Toolkit: https://github.com/Dejvino/pinetime-zephyr
* PineTime Zephyr-RTOS: https://github.com/Dejvino/zephyr.git (pinetime branch)

