#!/bin/bash

PATH=$PATH:/opt/SEGGER/JLink

$TERM JLinkRTTClient &
west debug --runner jlink --device NRF52


