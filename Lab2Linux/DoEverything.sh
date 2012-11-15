#!/bin/bash

#uninstall
rmmod pp_adc.ko
rm /dev/pp0adc

#clean/make
make kernel_clean
make

#install
insmod pp_adc.ko
mknod -m 666 /dev/pp0adc c 250 0
