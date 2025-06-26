# Noms des exécutables
NAME        = philo
NAME_BONUS  = philo_b

# Compilation
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror

# Sources et objets pour le mandatory
SRC         = init.c \
              routine.c \
              utils.c \
              cycle.c \
              main.c
OBJ_DIR     = obj
OBJ         = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))

# Sources et objets pour la bonus
SRC_BONUS       = philo_bonus/main.c \
                  philo_bonus/phylosopher.c \
                  philo_bonus/utils.c \
                  philo_bonus/sem_helper.c
OBJ_BONUS_DIR   = obj_bonus
OBJ_BONUS       = $(patsubst philo_bonus/%.c,$(OBJ_BONUS_DIR)/%.o,$(SRC_BONUS))

# Règle par défaut
all: $(NAME)

# Compilation du binaire mandatory
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Règle générique pour créer obj/%.o depuis %.c
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Création du dossier d’objets si nécessaire
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Règle pour la bonus
bonus: $(NAME_BONUS)

# Compilation du binaire bonus
$(NAME_BONUS): $(OBJ_BONUS)
	$(CC) $(CFLAGS) -o $@ $^

# Règle générique pour créer obj_bonus/%.o depuis philo_bonus/%.c
$(OBJ_BONUS_DIR)/%.o: philo_bonus/%.c | $(OBJ_BONUS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Création du dossier bonus si nécessaire
$(OBJ_BONUS_DIR):
	mkdir -p $(OBJ_BONUS_DIR)

# Nettoyage des objets
clean:
	rm -rf $(OBJ_DIR) $(OBJ_BONUS_DIR)

# Nettoyage complet (objets + exécutables)
fclean: clean
	rm -f $(NAME) $(NAME_BONUS)

# Rebuild complet
re: fclean all

.PHONY: all bonus clean fclean re
