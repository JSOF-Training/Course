




all: loadme loader_easier loader_harder


loadme:
	gcc -w -c main.c -o loadme.o
	gcc loadme.o -shared -o loadme.so

loader_easier: loader.o
	gcc loader.o -o loader_easier -Wl,--no-as-needed -l:libelf.so.1 -Wl,--as-needed -Wl,-rpath -Wl,.

loader_harder: loader.o
	gcc loader.o -o loader_harder
	@echo  "********make sure that DT_DEBUG entry is emitted in loader_harder. If not then add the linker flag -F some-value********"
	@echo  "********alternatively force DT_DEBUG to not be emitted with -z rodynamic and add -F some-value********"
	@echo  "********also make sure that there is a .comment section (also a .note section should work)********"
	@echo  "********The above are all just to make sure the solution in possible_solution.txt works********"
loader.o:
	gcc -w -c loader.c -o loader.o

clean:
	rm -f loadme.o loadme.so loader_easier loader_harder loader.o
