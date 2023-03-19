echo "Working directory:"
read dirname

if [ -n "$dirname" ]
then
	if [ -d "$dirname" ]
		then
			cd $dirname
			for dir in *
			do
				newname=`echo $dir | tr '[a-z] [A-Z]' '[A-Z] [a-z]'`
				if [ $newname != $dir ]
				then
					mv $dir $newname
				fi
			done
	else
		echo "ERROR! This Is Not Directory."
	fi
else
	for dir in *
	do
		newname=`echo $dir | tr '[a-z] [A-Z]' '[A-Z] [a-z]'`
		if [ $newname != $dir ]
		then
			mv $dir $newname
		fi
	done
fi
