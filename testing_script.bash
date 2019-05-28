prog="bin/c-format-checker"  # $1
src="091 Фибоначчиевы строки"  # $2

for i in "$src"/*
    do y=`eval "$prog" "\"$i\"" --quiet  --logfile="\"log/$(basename "$i").log\""`
    if [ "$?" -eq 1 ]  # если программа завершилась с сообщением об ошибке отступа
    then 
        echo "\"$i\""        # вывести имя файла
    fi
    # напечатать вывод программы в лог-файл
    echo "$y" > "out/$(basename "$i").log"
done > conclusion.txt  # вывести список файлов, для которых программа вернула код 1
