if [ $# -eq 0 ]
then
	echo "Usage:phone searchfor [...searchfor]"
	echo "(You didn't tell me what you want to search for.)"
else
	str="$1"
	for info in $*
	do
		str="$str|$info"
	done
	egrep "$str" mydata | awk -f display.awk
fi
  
