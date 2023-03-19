cat mydata > ans
if [ $# -eq 0 ]
then
	echo "Usage:phone searchfor [...searchfor]"
	echo "(You didn't tell me what you want to search for.)"
else
	str="$1"
	for info in $*
	do
		str="$str|$info"
		egrep "$info" ans > copy
		cat copy > ans
		rm copy
	done		
	cat ans | awk -f display.awk
	rm ans
fi
echo "or형식 출력"
egrep "$str" mydata | awk -f display.awk
 
