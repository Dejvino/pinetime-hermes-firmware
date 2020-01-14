# PineTime Hermes - Firmware
Firmware for the smart watch [PineTime](https://wiki.pine64.org/index.php/PineTime) from [PINE64](https://www.pine64.org/).
Primarily focused on displaying time and notifications.

Built using [Zephyr Project](https://zephyrproject.org/).

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
  
Enjoy!

# Current Status
* **clock** - renders green digital clock with low precision time updates
* **power management** - backlight is enabled by holding the button

# Related Links
* Hermes Companion App: https://github.com/Dejvino/pinetime-hermes-companion.git
* PineTime Zephyr Toolkit: https://github.com/Dejvino/pinetime-zephyr

