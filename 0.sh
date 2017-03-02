#!/bin/sh

cd $1
cmake --build . -- -j4
