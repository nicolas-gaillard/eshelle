# AUTEUR : Jordan BLOUIN
COMPI=./commands/execs
LIB=./commands/lib
DYNA=./commands/dynalib
MAIN=main#NOM DU FICHIER A DEFINIR

all: independance statique dynamique

#Règle d'effacement des fichiers en .o
clean:
	-@rm -rf *.o

# Compilation créant un exécutable pour chaque fichier C
independance: inde clean

inde: inde.o
	gcc -o $(COMPI)/ls ls.o
	gcc -o $(COMPI)/cat cat.o
	gcc -o $(COMPI)/cd cd.o
	gcc -o $(COMPI)/mkdir mkdir.o
	gcc -o $(COMPI)/pwd pwd.o
	gcc -o $(COMPI)/du du.o
	gcc -o $(COMPI)/rm rm.o
	gcc -o $(COMPI)/mv mv.o
	gcc -o $(COMPI)/cp cp.o
	gcc -o $(COMPI)/chmod chmod.o
	gcc -o $(COMPI)/chown chown.o
	gcc -o $(COMPI)/chgrp chgrp.o
	gcc -o $(COMPI)/echo echo.o
	

# Compilation en librairie statique intégré à l'exécutable
statique : stat clean

stat : stat.o
	ar crs $(LIB)/libcommands.a ls.o cat.o cd.o mkdir.o pwd.o du.o rm.o mv.o cp.o chmod.o chown.o chgrp.o echo.o
	ranlib $(LIB)/libcommands.a
	gcc -I$(LIB) -L$(LIB) -O $(LIB)/$(MAIN).c -o $(LIB)/$(MAIN) -lcommands
	
# Compilation en librairie dynamique à chargement explicite
dynamique: dyna clean

dyna: dyna.o
	gcc -shared -o $(DYNA)/libcommands.so IS_file.o cat.o cd.o mkdir.o pwd.o du.o rm.o mv.o cp.o chmod.o chown.o chgrp.o echo.o

inde.o :
	gcc -c $(COMPI)/ls.c
	gcc -c $(COMPI)/cat.c
	gcc -c $(COMPI)/cd.c
	gcc -c $(COMPI)/mkdir.c
	gcc -c $(COMPI)/pwd.c
	gcc -c $(COMPI)/du.c
	gcc -c $(COMPI)/rm.c
	gcc -c $(COMPI)/mv.c
	gcc -c $(COMPI)/cp.c
	gcc -c $(COMPI)/chmod.c
	gcc -c $(COMPI)/chown.c
	gcc -c $(COMPI)/chgrp.c
	gcc -c $(COMPI)/echo.c
	
stat.o :
	gcc -c $(LIB)/ls.c
	gcc -c $(LIB)/cat.c
	gcc -c $(LIB)/cd.c
	gcc -c $(LIB)/mkdir.c
	gcc -c $(LIB)/pwd.c
	gcc -c $(LIB)/du.c
	gcc -c $(LIB)/rm.c
	gcc -c $(LIB)/mv.c
	gcc -c $(LIB)/cp.c
	gcc -c $(LIB)/chmod.c
	gcc -c $(LIB)/chown.c
	gcc -c $(LIB)/chgrp.c
	gcc -c $(LIB)/echo.c

dyna.o:
	gcc -fPIC -c $(DYNA)/IS_file.c
	gcc -fPIC -c $(DYNA)/ls.c
	gcc -fPIC -c $(DYNA)/cat.c
	gcc -fPIC -c $(DYNA)/cd.c
	gcc -fPIC -c $(DYNA)/mkdir.c
	gcc -fPIC -c $(DYNA)/pwd.c
	gcc -fPIC -c $(DYNA)/du.c
	gcc -fPIC -c $(DYNA)/rm.c
	gcc -fPIC -c $(DYNA)/mv.c
	gcc -fPIC -c $(DYNA)/cp.c
	gcc -fPIC -c $(DYNA)/chmod.c
	gcc -fPIC -c $(DYNA)/chown.c
	gcc -fPIC -c $(DYNA)/chgrp.c
	gcc -fPIC -c $(DYNA)/echo.c