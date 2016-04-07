#!/usr/bin/env bash

git clone https://EricBotter@github.com/EricBotter/WebReplication || exit 1
mkdir Thesis
cd Thesis
cmake ../WebReplication
make
