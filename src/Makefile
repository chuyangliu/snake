OBJS = main.o Console.o GameCtrl.o Grid.o SearchableGrid.o \
Map.o Point.o Snake.o Convert.o

CC = g++ -std=c++11

LIB = -pthread

APP_NAME = Snake

$(APP_NAME): $(OBJS)
	$(CC) -o $(APP_NAME) $(OBJS) $(LIB)

$(OBJS): %.o: %.cpp
	$(CC) -c $< -o $@

clean:
	rm $(APP_NAME) ${OBJS}

run:
	@./$(APP_NAME)