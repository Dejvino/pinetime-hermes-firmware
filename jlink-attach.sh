#!/bin/bash

MINITERM=$TERM
ELF_FILE="build/zephyr/zephyr.elf"

$MINITERM -e "sleep 2; arm-none-eabi-gdb $ELF_FILE -ex 'target remote localhost:2331'" &

JLinkGDBServerCLExe -device nrf52 -if swd -speed 4000 -autoconnect 1

