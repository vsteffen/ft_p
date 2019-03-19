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
BIN_SRV	=	serveur
BIN_CLT	=	client
NAME	=	$(BIN_SRV) $(BIN_CLT)

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

CFLAGS = -Wall -Wextra -g -fsanitize=address

LIB		=	$(ROOT)/lib
LIBSRCS	=	$(ROOT)/libsrcs
LIBFT 	=	$(LIBSRCS)/libft
MD5 	=	$(LIBSRCS)/md5

ROOT  	=	$(shell /bin/pwd)
OPATH 	=	$(ROOT)/objs
CPATH 	=	$(ROOT)/srcs
SRVPATH	=	/server
CLTPATH	=	/client
LPATH	=	$(LIBFT)/libft.a $(MD5)/md5.a
HPATH 	=	-I $(ROOT)/includes -I $(LIBFT)/includes

SRC_SRV	=	$(SRVPATH)/main.c \
			$(SRVPATH)/handle_all_connection_srv.c \
			$(SRVPATH)/signals_srv.c \
			$(SRVPATH)/handle_request.c \
			$(SRVPATH)/auth.c

SRC_CLT	=	$(CLTPATH)/main.c \
			$(CLTPATH)/handle_connection_clt.c \
			$(CLTPATH)/signals_clt.c \
			$(CLTPATH)/handle_cmd.c

SRC	=	$(SRC_SRV) $(SRC_CLT)

PRE_CHECK_SUB_LIBFT	=	$(LIBFT)/Makefile
PRE_CHECK_SUB	=	$(PRE_CHECK_SUB_LIBFT)
PRE_CHECK_LIB_LIBFT	=	$(LIBFT)/libft.a
PRE_CHECK_LIB_MD5	=	$(MD5)/md5.a
PRE_CHECK_LIB	=	$(PRE_CHECK_LIB_LIBFT)

COMPILE = no

OS		:= $(shell uname -s)

ifeq ($(OS),Darwin)
	NPROCS:=$(shell sysctl -n hw.ncpu)
else
	NPROCS:=$(shell grep -c ^processor /proc/cpuinfo)
endif

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

.PHONY: all clean fclean re lib-clean lib-update

all: $(NAME)


$(PRE_CHECK_SUB):
	@echo $(PROJECT)": Init submodules ... "
	@$(GIT) submodule init > /dev/null  # can't directly redirect stdout on /dev/null cause of sync wait on Linux
	@$(GIT) submodule update --recursive --remote > /dev/null
	@printf $(PROJECT)": submodules "
	@$(call PRINT_STATUS,INITIALIZED,SUCCESS)

$(PRE_CHECK_LIB): $(PRE_CHECK_SUB)
	@echo $(PROJECT)": Compile libraries ... "
	$(if $(filter $(OS),Darwin),@$(MAKE) -C $(LIBFT) -j$(NPROCS) > /dev/null,@$(MAKE) -C $(LIBFT) no-asm -j$(NPROCS) > /dev/null)
	@$(MAKE) -C $(MD5) -j$(NPROCS) > /dev/null
	@printf $(PROJECT)": all libraries "
	@$(call PRINT_STATUS,COMPILED,SUCCESS)

$(NAME): $(PRE_CHECK_LIB) $(OPATH) $(OBJ)
	$(if $(filter $(COMPILE),yes),@echo ']')
	@echo $(PROJECT)": Building $(BIN_SRV) ... "
	@$(CC) -o $(BIN_SRV) $(CFLAGS) $(OBJ_SRV) $(LPATH) $(HPATH)
	@echo $(PROJECT)": Building $(BIN_CLT) ... "
	@$(CC) -o $(BIN_CLT) $(CFLAGS) $(OBJ_CLT) $(LPATH) $(HPATH)
	@printf $(PROJECT)": all binaries "
	@$(call PRINT_STATUS,DONE,SUCCESS)

$(OPATH)/%.o: $(CPATH)/%.c | $(PRE_CHECK_SUB)
	$(if $(filter $(COMPILE),no),@printf $(PROJECT)': Files compiling [')
	@$(eval COMPILE := yes)
	@$(CC) $(CFLAGS) -c $< -o $@ $(HPATH)
	$(call PRINT_GREEN,.)

$(OPATH):
	@echo $(PROJECT)": Creation of objects directory"
	@$(MKDIR) $@ $@$(SRVPATH) $@$(CLTPATH)

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

re: fclean
	@$(MAKE) -C $(ROOT) -j$(NPROCS)

lib-clean:
	@echo $(PROJECT)": cleaning libraries ..."
	@-$(MAKE) -C $(LIBFT) fclean -j$(NPROCS) > /dev/null
	@-$(MAKE) -C $(MD5) fclean -j$(NPROCS) > /dev/null
	@printf $(PROJECT)": $@ rule "
	@$(call PRINT_STATUS,DONE,SUCCESS)

lib-update:
	@echo $(PROJECT)": Update submodules ... "
	@$(GIT) submodule update --recursive --remote > /dev/null
	@printf $(PROJECT)": submodules "
	@$(call PRINT_STATUS,UPDATED,SUCCESS)
