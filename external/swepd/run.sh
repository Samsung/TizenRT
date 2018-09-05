#!/bin/bash

make clean
make

date
./run_SR_EPD ../sample.pcm output.txt
date

