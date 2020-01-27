#!/bin/bash
#
#	Test Initial Repository Setup
#
# This is an automated test script that simulates a new user trying out this repository.
# It should be run once in a while to check that the installation steps work even for
# a freshly cloned repository. It could also serve as a reference setup.
#
# NOTE: this test assumes the zephyr tool 'west' is already installed and available.
# Follow the official steps to set it up first:
#  https://docs.zephyrproject.org/latest/getting_started/index.html
#

# cleanup from previous attempt
rm -rf ~/tmp/pinetime-hermes-firmware

# prepare a testing directory
mkdir -p ~/tmp
cd ~/tmp

# clone the repository with all its submodules
git clone --recurse-submodules https://github.com/Dejvino/pinetime-hermes-firmware
cd pinetime-hermes-firmware

# setup project dependencies via west
west update

# build the project
west build -p -b pinetime app && echo "TEST PASSED!"

