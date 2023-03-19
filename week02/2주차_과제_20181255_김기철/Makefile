cc = gcc
target = Main
objects = Main.o RecursiveFunc.o CountFunc.o ArrayFunc.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

$(objects) : Header.h

.PHONY : clean
clean :
	rm $(target) $(objects)
