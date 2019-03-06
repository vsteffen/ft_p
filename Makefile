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

PROJECT	=	ft_p
NAME	=	ft_p
BIN_SRV	=	serveur
BIN_CLT	=	client

CC 		=	/usr/bin/clang
RM 		=	/bin/rm
MAKE 	=	/usr/bin/make
MKDIR 	=	/bin/mkdir -p
AR 		=	/usr/bin/ar
RANLIB 	=	/usr/bin/ranlib
GIT		=	/usr/bin/git

UNAME_S := $(shell uname -s)

OBJ_SRV	=	$(patsubst %.c, $(OPATH)/%.o, $(SRC_SRV))
OBJ_CLT	=	$(patsubst %.c, $(OPATH)/%.o, $(SRC_CLT))

OBJ = $(OBJ_SRV) $(OBJ_CLT)

CFLAGS = -Wall -Wextra -Werror -g

LIB		=	$(ROOT)/lib
LIBSRCS	=	$(ROOT)/libsrcs
LIBFT 	=	$(LIBSRCS)/libft

ROOT  	=	$(shell /bin/pwd)
OPATH 	=	$(ROOT)/objs
CPATH 	=	$(ROOT)/srcs
SRVPATH	=	/server
CLTPATH	=	/client
LPATH	=	$(LIBFT)/libft.a
HPATH 	=	-I $(ROOT)/includes -I $(LIBFT)/includes

SRC_SRV	=	$(SRVPATH)/main.c \
			$(SRVPATH)/handle_connection_srv.c \
			$(SRVPATH)/signals_srv.c

SRC_CLT	=	$(CLTPATH)/main.c \
			$(CLTPATH)/handle_connection_clt.c \
			$(CLTPATH)/signals_clt.c

SRC	=	$(SRC_SRV) $(SRC_CLT)

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

.PHONY: all clean fclean re pre-check-submodule pre-check-lib lib-clean

all: $(NAME)

pre-check-submodule:
	@echo $(PROJECT)": Init and update submodules ... "
	@$(GIT) submodule init > /dev/null  # can't directly redirect stdout on /dev/null cause of sync wait on Linux
	@$(GIT) submodule update --recursive --remote > /dev/null
	@printf $(PROJECT)": pre-check-submodule rule "
	@$(call PRINT_STATUS,UP-TO-DATE,SUCCESS)

pre-check-lib: pre-check-submodule
	@echo $(PROJECT)": Compile and verify libraries ... "
	$(if $(filter $(UNAME_S),Darwin),@$(MAKE) -C $(LIBFT) > /dev/null,@$(MAKE) no-asm -C $(LIBFT) > /dev/null)
	@printf $(PROJECT)": pre-check-lib rule "
	@$(call PRINT_STATUS,UP-TO-DATE,SUCCESS)

$(NAME): pre-check-submodule pre-check-lib $(OPATH) $(OBJ)
	$(if $(filter $(COMPILE),yes),@echo ']')
	@printf $(PROJECT)": Building $(BIN_SRV) ... "
	@$(CC) -o $(BIN_SRV) $(CFLAGS) $(OBJ_SRV) $(LPATH) $(HPATH)
	@$(call PRINT_STATUS,DONE,SUCCESS)
	@printf $(PROJECT)": Building $(BIN_CLT) ... "
	@$(CC) -o $(BIN_CLT) $(CFLAGS) $(OBJ_CLT) $(LPATH) $(HPATH)
	@$(call PRINT_STATUS,DONE,SUCCESS)

$(OPATH)/%.o: $(CPATH)/%.c | pre-check-submodule pre-check-lib
	$(if $(filter $(COMPILE),no),@printf $(PROJECT)': Files compiling [')
	@$(eval COMPILE := yes)
	@$(CC) $(CFLAGS) -c $< -o $@ $(HPATH)
	$(call PRINT_GREEN,.)

$(OPATH):
	@$(MKDIR) $@ $@$(SRVPATH) $@$(CLTPATH)
	@echo $(PROJECT)": Directory for objects created"

clean:
	@$(RM) -Rf $(OPATH)
	@echo $(PROJECT)": Objects cleaned "
	@printf $(PROJECT)": clean rule "
	@$(call PRINT_STATUS,DONE,SUCCESS)

fclean: clean
	@$(RM) -f $(BIN_SRV) $(BIN_CLT)
	@echo $(PROJECT)": executables clean"
	@printf $(PROJECT)": fclean rule "
	@$(call PRINT_STATUS,DONE,SUCCESS)

re: fclean all

lib-clean:
	@-$(MAKE) fclean -C $(LIBFT) > /dev/null
	@echo $(PROJECT)": libraries fcleaned"
	@printf $(PROJECT)": lib-clean rule "
	@$(call PRINT_STATUS,DONE,SUCCESS)
