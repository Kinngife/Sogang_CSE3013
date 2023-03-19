cc = g++
target = main
objects = main.o Str.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

$(objects) : Str.h

.PHONY : clean
clean :
	rm $(target) $(objects)
