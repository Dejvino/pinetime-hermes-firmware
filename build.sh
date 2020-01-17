#!/bin/bash

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"

west build -p -b pinetime $SCRIPTPATH/app

