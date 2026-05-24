
TARGET  = jogo
SRC     = main.c
 
CFLAGS  = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 --cflags)
LIBS    = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 --libs)
  
all: $(TARGET)
 
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)
 
clean:
	rm -f $(TARGET)