# PineTime Hermes - Firmware
Firmware for the smart watch [PineTime](https://wiki.pine64.org/index.php/PineTime) from [PINE64](https://www.pine64.org/).
Primarily focused on displaying time and notifications.

Built using [Zephyr Project](https://zephyrproject.org/).

> **Hermes**, messenger of the gods.
> He can convey messages between the divine realms, the underworld, and the world of mortals.

# Getting Started
1. Install Zephyr dependencies: https://docs.zephyrproject.org/latest/getting_started/index.html
  * Note: use this repository's directory whenever you see mention of '~/zephyrproject'
  1. OS Update
  2. Install dependencies
  3. Get the source code (don't create a new project, clone this repository!)
     ```
     cd ~/
     git clone --recursive https://github.com/Dejvino/pinetime-hermes-firmware.git
     cd pinetime-hermes-firmware
     west update
     ```
  4. Install needed Python packages
  5. Install Software Development Toolchain
2. Build the App
  ```
  cd ~/pinetime-hermes-firmware
  west build -p -b pinetime ./app
  ```
3. Flash the App
  ```
  west flash
  ```
4. Enjoy!

# Related links
* Hermes Companion App: https://github.com/Dejvino/pinetime-hermes-companion.git
* PineTime Zephyr Toolkit: https://github.com/Dejvino/pinetime-zephyr

