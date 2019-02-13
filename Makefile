# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vsteffen <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/02/06 20:52:05 by vsteffen          #+#    #+#              #
#    Updated: 2018/03/20 18:17:27 by vsteffen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

PROJECT	= ft_p
NAME	= ft_p

CC 		=	/usr/bin/clang
RM 		=	/bin/rm
MAKE 	=	/usr/bin/make
MKDIR 	=	/bin/mkdir -p
AR 		=	/usr/bin/ar
RANLIB 	=	/usr/bin/ranlib
GIT		=	/usr/bin/git

OBJ = $(patsubst %.c, $(OPATH)/%.o, $(SRC))

CFLAGS = -Wall -Wextra -Werror -g

LIB		=	$(ROOT)/lib
LIBSRCS	=	$(ROOT)/libsrcs
LIBFT 	=	$(LIBSRCS)/libft

ROOT  	=	$(shell /bin/pwd)
OPATH 	=	$(ROOT)/objs
CPATH 	=	$(ROOT)/srcs
LPATH	=	$(LIBFT)/libft.a
HPATH 	=	-I $(ROOT)/includes -I $(LIBFT)/includes

SRC =	ft_p.c


COMPILE = no

define PRINT_RED
    @printf "\033[31m$(1)\033[0m"
endef

define PRINT_GREEN
    @printf "\033[32m$(1)\033[0m"
endef

define PRINT_YELLOW
    @printf "\033[33m$(1)\033[0m"
endef

define PRINT_STATUS
	@printf '['
	$(if $(filter $(2),SUCCESS),$(call PRINT_GREEN,$(1)))
	$(if $(filter $(2),FAIL),$(call PRINT_RED,$(1)))
	$(if $(filter $(2),WARN),$(call PRINT_YELLOW,$(1)))
	$(if $(filter $(2),INFO),printf $(1))
	$(if $(filter $(3),-n),printf $(1),echo ']')
endef

.PHONY: all clean fclean re

all: pre-check-submodule pre-check-lib $(NAME)

pre-check-submodule:
	@printf $(PROJECT)": Init and update submodules ... "
	@$(GIT) submodule init &>/dev/null
	@$(GIT) submodule update --recursive --remote &>/dev/null
	@$(call PRINT_STATUS,UP-TO-DATE,SUCCESS)

pre-check-lib:
	@printf $(PROJECT)": Compile and verify libraries ... "
	@$(MAKE) -C $(LIBFT) &>/dev/null
	@$(call PRINT_STATUS,UP-TO-DATE,SUCCESS)

$(NAME): $(OPATH) $(OBJ)
	$(if $(filter $(COMPILE),yes),@echo ']')
	@printf $(PROJECT)": Building $@ ... "
	@$(CC) -o $@ $(CFLAGS) $(OBJ) $(LPATH) $(HPATH)
	@$(call PRINT_STATUS,DONE,SUCCESS)

$(OPATH)/%.o: $(CPATH)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(HPATH)
	$(if $(filter $(COMPILE),no),@printf $(PROJECT)': Files compiling [')
	@$(eval COMPILE := yes)
	$(call PRINT_GREEN,.)

$(OPATH):
	@$(MKDIR) $@
	@echo $(PROJECT)": Directory for objects created"

clean:
	@$(RM) -Rf $(OPATH)
	@echo $(PROJECT)": Objects cleaned "
	@printf $(PROJECT)": clean rule "
	@$(call PRINT_STATUS,DONE,SUCCESS)

fclean: clean
	@$(RM) -f $(NAME)
	@echo $(PROJECT)": executable clean"
	@$(MAKE) fclean -C $(LIBFT) &>/dev/null
	@echo $(PROJECT)": libraries fcleaned"
	@printf $(PROJECT)": fclean rules "
	@$(call PRINT_STATUS,DONE,SUCCESS)

re: fclean all
