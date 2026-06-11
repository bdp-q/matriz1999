
TARGET  = jogo
SRC     = Main.c Entities/Player.c Entities/Hitbox.c Entities/Danger.c Joystick.c Rooms/Room.c
 
CFLAGS  = -I. -IEntities -IRooms $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --cflags)
LIBS    = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs) -lm
  
all: $(TARGET)
 
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)
 
clean:
	rm -f $(TARGET)