# AUTEUR : Jordan BLOUIN
COMPI=./commands

all: independance librairie

# Compilation créant un exécutable pour chaque fichier C
independance: inde clean

inde:
	gcc -c -Wall $(COMPI)/IS_file.c
	gcc -c -Wall $(COMPI)/ls.c
	gcc -c -Wall $(COMPI)/cat.c
	gcc -c -Wall $(COMPI)/cd.c
	gcc -c -Wall $(COMPI)/mkdir.c
	gcc -c -Wall $(COMPI)/pwd.c
	gcc -o $(COMPI)/IS_file IS_file.o
	gcc -o $(COMPI)/ls ls.o
	gcc -o $(COMPI)/cat cat.o
	gcc -o $(COMPI)/cd cd.o
	gcc -o $(COMPI)/mkdir mkdir.o
	gcc -o $(COMPI)/pwd pwd.o

# Compilation en librairie dynamique à chargement explicite
librairie: lib clean

lib:
	gcc -fPIC -c -Wall $(COMPI)/IS_file.c
	gcc -fPIC -c -Wall $(COMPI)/ls.c
	gcc -fPIC -c -Wall $(COMPI)/cat.c
	gcc -fPIC -c -Wall $(COMPI)/cd.c
	gcc -fPIC -c -Wall $(COMPI)/mkdir.c
	gcc -fPIC -c -Wall $(COMPI)/pwd.c
	gcc -shared -o $(COMPI)/libIS_file.so IS_file.o 
	gcc -shared -o $(COMPI)/libcat.so cat.o
	gcc -shared -o $(COMPI)/libcd.so cd.o
	gcc -shared -o $(COMPI)/libmkdir.so mkdir.o
	gcc -shared -o $(COMPI)/libpwd.so pwd.o
	gcc -shared -o $(COMPI)/libcommandes.so $(COMPI)/libIS_file.so $(COMPI)/libcat.so $(COMPI)/libcd.so $(COMPI)/libmkdir.so $(COMPI)/libpwd.so

clean:
	-@rm -rf *.o