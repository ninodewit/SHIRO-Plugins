#!/bin/bash

# make audio ports constant, they must never change
sed -i -e "/^int gen_kernel_numins = /{s/^/const /}" plugins/*/gen_exported.cpp
sed -i -e "/^int gen_kernel_numouts = /{s/^/const /}" plugins/*/gen_exported.cpp

# change prototype of param functions, no doubles!
sed -i -e "s/long index, double value,/long index, t_param value,/" plugins/*/gen_exported.cpp plugins/*/gen_exported.h
sed -i -e "s/long index, double \*value/long index, t_param \*value/" plugins/*/gen_exported.cpp plugins/*/gen_exported.h
sed -i -e "s/create(double sr, long vs)/create(t_param sr, long vs)/" plugins/*/gen_exported.cpp plugins/*/gen_exported.h

# use t_sample instead of forcing doubles
sed -i -e "s/double /t_sample /" plugins/*/gen_exported.cpp
