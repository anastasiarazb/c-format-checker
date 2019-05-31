prog="c-format-checker/bin/c-format-checker"  # $1
src="091 Фибоначчиевы строки"  # $2
src="094 Наибольший простой делитель"
#src="148 Полином"

for i in "$src"/*
    do y=`eval "$prog" "\"$i\"" --quiet  --logfile="\"log/$(basename "$i").log\""`
    res="$?"
    if [ $res -eq 1 ]  # если программа завершилась с сообщением об ошибке отступа
    then 
        echo "$i"        # вывести имя файла
    fi
    if [ $res -eq 5 ]  # если программа завершилась с сообщением об ошибке на этапе парсинга
    then 
        echo "$i" >> parse_errors.log       # вывести имя файла в parse_errors.log
    fi
    # напечатать вывод программы в лог-файл
    echo "$y" > "out/$(basename "$i").log"
done > conclusion.txt  # вывести список файлов, для которых программа вернула код 1
