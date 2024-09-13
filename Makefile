MAKEFLAGS += -s

NAME = cub3D

LIBFT = ./libs/libft.a
MINILIBX = ./libs/minilibx-linux/libmlx_Linux.a

CC = cc -g -O3 -ffast-math
CCFLAGS = #-Wall -Werror -Wextra
MLXFLAGS = -L ./libs/minilibx-linux -lm -lmlx -Ilmlx -lXext -lX11
RM = rm
RMFLAGS= -f
GREEN=\033[0;32m
RED=\033[0;31m
YELLOW=\033[1;93m
NC=\033[0m


SRC = main.c \
	src/key_handler.c \
	src/free.c \
	src/fill_map.c \
	src/debug_fuctions.c \
	src/shoot_rays.c \
	src/draw_wall.c \
	src/place_player.c \
	src/init.c \
	src/map_check.c \
	src/texture_handler/create_texture.c\
	#src/map_check.c

OBJ_DIR = obj
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(LIBFT):
		$(MAKE) -C ./libs/

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) -c $< -o $@

$(NAME): $(OBJ) $(LIBFT) $(MINILIBX)
	$(CC) $(CCFLAGS) $(OBJ) $(LIBFT) $(MLXFLAGS) -o $(NAME) -lreadline


	@echo "\n${YELLOW}--------------------------"
	@echo "${YELLOW}| ${GREEN}All files are compiled ${YELLOW}|"
	@echo "${YELLOW}--------------------------${NC}\n"


clean:
	$(RM) $(RMFLAGS) $(OBJ) $(LIBFT)
	$(MAKE) -C ./libs/ clean
	$(RM) $(RMFLAGS) -r $(OBJ_DIR)

	@echo "\n${YELLOW}-----------------------------"
	@echo "${YELLOW}| ${GREEN}Cleaned all ${RED}program${GREEN} files ${YELLOW}|"
	@echo "${YELLOW}-----------------------------${NC}\n"

fclean: clean
	$(RM) $(RMFLAGS) $(NAME) $(OBJ) $(LIBFT) $(OBJ_DIR)
	$(MAKE) -C ./libs/ fclean

	@echo "\n${YELLOW}--------------------------------"
	@echo "${YELLOW}| ${RED}ALL files ${GREEN}have been cleaned! ${YELLOW}|"
	@echo "${YELLOW}--------------------------------${NC}\n"

$(MINILIBX):
	$(MAKE) -C ./libs/minilibx-linux

norminette:
	@norminette | grep -v "line too long" | grep -v "Comment is invalid in this scope" | grep -v "libs"

valgrind:
	@make re
	@valgrind --quiet --leak-check=full --show-leak-kinds=all --track-origins=yes --tool=memcheck ./cub3d
run:
	@make
	@./cub3D

update:
	@wget https://cdn.intra.42.fr/document/document/25858/minilibx-linux.tgz
	@tar -xzf minilibx-linux.tgz -C libs
	@rm minilibx-linux.tgz

re: fclean all


