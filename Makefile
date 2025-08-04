NAME        = philos
NAME_BONUS  = philos_bonus
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror

SRC			= philo/cycle.c \
			  philo/init.c \
			  philo/main.c \
			  philo/routine.c \
			  philo/utils.c
OBJ_DIR     = obj
OBJ         = $(patsubst philo/%.c,$(OBJ_DIR)/%.o,$(SRC))

SRC_BONUS       = philo_bonus/main.c \
                  philo_bonus/phylosopher.c \
                  philo_bonus/utils.c \
                  philo_bonus/sem_helper.c \
				  philo_bonus/utils_main.c \
				  philo_bonus/utils2.c 
OBJ_BONUS_DIR   = obj_bonus
OBJ_BONUS       = $(patsubst philo_bonus/%.c,$(OBJ_BONUS_DIR)/%.o,$(SRC_BONUS))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: philo/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJ_BONUS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_BONUS_DIR)/%.o: philo_bonus/%.c | $(OBJ_BONUS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_BONUS_DIR):
	mkdir -p $(OBJ_BONUS_DIR)

clean:
	rm -rf $(OBJ_DIR) $(OBJ_BONUS_DIR)

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)

re: fclean all bonus

.PHONY: all bonus clean fclean re
