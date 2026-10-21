#!/bin/bash

# 指定要遍历的目录
DIRECTORY="example"
out_dir="example_output"

function build()
{
    # 遍历目录中的所有文件
    for FILE in "$DIRECTORY"/*.c
    do
        # 检查是否为文件
        if [ -f "$FILE" ]; then
            # 提取文件名（不带路径）
            FILENAME=$(basename "$FILE")
            
            # 调用 make 并传递文件名
            echo "Compiling $FILENAME"
            make APP_SRC="$FILENAME"

            # 检查 make 的返回状态
            if [ $? -ne 0 ]; then
                echo "Error: Compilation failed for $filename"
            else
                filename=$(basename "$FILE" .c)
                # echo "cp $filename"
                mv "$filename" "$out_dir"
            fi
        fi
    done
}

function clean()
{
    # 遍历目录中的所有文件
    for FILE in "$DIRECTORY"/*.c
    do
        # 检查是否为文件
        if [ -f "$FILE" ]; then
            # 提取文件名（不带路径）
            FILENAME=$(basename "$FILE")
            
            # 调用 make 并传递文件名
            echo "Compiling $FILENAME"
            make APP_SRC="$FILENAME" clean

            # 检查 make 的返回状态
            if [ $? -ne 0 ]; then
                echo "Error: Compilation failed for $filename"
            fi
        fi
    done
    rm -rf $out_dir
}

if [ ! -d "$out_dir" ]; then
    mkdir -p "$out_dir"
fi

case $1 in
    all)
        build
        ;;
    clean)
        clean
        ;;
    help)
        echo "Usage: $0 {all|clean|help|no parameters}"
        ;;
    *)
        build
        ;;
esac
