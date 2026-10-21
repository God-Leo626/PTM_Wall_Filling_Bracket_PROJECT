#!/bin/bash

i2c_dir="NDS03_Linux_I2C"
input_dir="NDS03_Linux_Input"
out_dir="output"

function build_i2c()
{
    original_dir=$(pwd)
    cd $i2c_dir
    make
    make_status=$?
    if [ $make_status -eq 0 ]; then
        cp -r "${i2c_dir}_output" "${original_dir}/${out_dir}"
    else
        echo -e "\033[31m    i2c build failed \033[0m"
    fi
    cd $original_dir
}

function build_input()
{
    original_dir=$(pwd)
    cd $input_dir
    make
    make_status=$?
    if [ $make_status -eq 0 ]; then
        cp -r "${input_dir}_output" "${original_dir}/${out_dir}"
    else
        echo -e "\033[31m    input build failed \033[0m"
    fi
    cd $original_dir
}

function clean_i2c()
{
    original_dir=$(pwd)
    cd $i2c_dir
    make clean
    cd $original_dir
}

function clean_input()
{
    original_dir=$(pwd)
    cd $input_dir
    make clean
    cd $original_dir
}

function build_all()
{
    build_i2c
    build_input
}

function clean_all()
{
    clean_i2c
    clean_input

    rm -rf $out_dir
}


if [ ! -d "$out_dir" ]; then
    mkdir -p "$out_dir"
fi

case $1 in
    all)
        build_all
        ;;
    clean)
        clean_all
        ;;
    cfg)
        build_all_cfg
        ;;
    help)
        echo "Usage: $0 {all|clean|cfg|help|no parameters}"
        ;;
    *)
        build_all
        ;;
esac