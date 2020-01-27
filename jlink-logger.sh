#!/bin/bash

MINITERM=$TERM


$MINITERM JLinkRTTClient &

JLinkExe -device nrf52 -if swd -speed 4000 -autoconnect 1

