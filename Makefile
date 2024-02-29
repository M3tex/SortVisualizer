SRC := ./src
BUILD := ./build
INCLUDE := ./include


GTK_FLAGS := `pkg-config --cflags gtk4`
GTK_LIBS := `pkg-config --libs gtk4`

C_FLAGS := $(GTK_FLAGS) -I $(INCLUDE) -Wall -g
C_LIBS := $(GTK_LIBS) -lm -lpthread

C_FILES := $(wildcard $(SRC)/*.c)
HEADERS := $(wildcard $(INCLUDE)/*.h)
OBJS := $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(C_FILES))


all: $(OBJS) $(HEADERS) $(C_FILES) sortV

# Un fichier objet dépend de sa source mais également des headers.
# Ainsi si un header est modifié on recompile tous les objets pour être sûrs
# que les changements soient bien appliqués partout
$(BUILD)/%.o: $(SRC)/%.c $(HEADERS)
	gcc $(C_FLAGS) -c $< -o $@ $(C_LIBS)

sortV: $(OBJS)
	gcc $(C_FLAGS) -o sortV $(OBJS) $(C_LIBS)

vg: sortV
	valgrind --suppressions=/usr/share/glib-2.0/valgrind/glib.supp --suppressions=/usr/share/gtk-4.0/valgrind/gtk.supp --leak-check=full --log-file=vgdump ./sortV

clean:
	rm ./build/*
	rm sortV
