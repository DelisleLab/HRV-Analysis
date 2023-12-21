file=$1
name="${file%.*}"
echo $name
ext="${file##*.}"
echo $ext

asc2bin $file

mv X.bin $name.bin
