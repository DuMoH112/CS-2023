#!/bin/sh
# Скрипт для запуска выбранного файла с расширением .asm в папке tasks

cd tasks
mkdir build

echo -n "Выберите режим:\n1. Запуск\n2. Отладка\nSelect: "
read mode

echo -n "\nВыберите файл для запуска из списка:\n"
echo "----------" && ls -1 *.asm && echo -n "----------\nSelect: "
read file

if [ -f $file ]; then
    # Компиляция
    nasm -f elf32 -F dwarf -g $file -o build/${file%.asm}.o
    ld -m elf_i386 -o build/${file%.asm} build/${file%.asm}.o

    # Запуск в соответсвии с режимом
    if [ $mode -eq 1 ]; then
        echo "Результат: "
        ./build/${file%.asm}
    elif [ $mode -eq 2 ]; then
        echo "break _start" >.gdbinit # Создаем файл .gdbinit и записываем в него команды для gdb
        echo "layout regs" >>.gdbinit
        echo "run" >>.gdbinit
        gdb -iex "set auto-load safe-path /source/tasks" ./build/${file%.asm}
    else
        echo "\nНеверный режим"
    fi

else
    echo "\nФайл не найден"
fi
