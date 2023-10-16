# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra

# Noms des fichiers source
SOURCES = main.c bankfunctions.c

# Noms des fichiers objets générés
OBJECTS = $(SOURCES:.c=.o)

# Nom du binaire final
EXECUTABLE = myprogram

# Règle par défaut
all: $(EXECUTABLE)

# Compilation du binaire
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation des fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Nettoyage des fichiers objets et du binaire
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
