cc=gcc
target=animal_exe
objects=dog.o blackcow.o turtle.o main.o

$(target): $(objects)
	$(cc) -o $(target) $(objects)

.PHONY : clean
clean :
	rm $(target) $(objects)
