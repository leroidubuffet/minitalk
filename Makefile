# Variables
NAME = Minitalk

CC = cc
CCFLAGS = -Wall -Wextra -Werror -g

INCLUDES = -I include/

RM = rm -f

SRC_SRV = src/server.c
SRC_CLIENT = src/client.c

CLIENT = client
SERVER = server

SRC_COMMON = src/common.c
COMMON = common.o

# Rules
all: $(NAME)

$(NAME): $(CLIENT) $(SERVER)

$(COMMON): $(SRC_COMMON)
	@$(CC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<
	@echo "$@ compilation succesful"

$(CLIENT): $(SRC_CLIENT) $(COMMON)
	@$(CC) $(CCFLAGS) $(INCLUDES) $(SRC_CLIENT) $(COMMON) -o $(CLIENT)
	@echo "$(CLIENT) created"

$(SERVER): $(SRC_SRV) $(COMMON)
	@$(CC) $(CCFLAGS) $(INCLUDES) $(SRC_SRV) $(COMMON) -o $(SERVER)
	@echo "$(SERVER) created"

clean:
	@$(RM) $(COMMON)
	@echo "$(COMMON) deleted"

fclean: clean
	@$(RM) $(CLIENT) $(SERVER)
	@echo "$(NAME) deleted"

re: fclean all

.PHONY: all clean fclean re
