# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mateon <mateon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/12 17:47:46 by mateon            #+#    #+#              #
#    Updated: 2026/03/06 09:16:19 by mateon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

CC			=	cc

CFLAGS		=	-Wall -Wextra -Werror


# Linux flags
#LDFLAGS		=	-lreadline
# Mac flags
READLINE_DIR	=	$(shell brew --prefix readline)
LDFLAGS		=	-L$(READLINE_DIR)/lib -lreadline

RM			=	rm -f

OBJ_DIR		=	obj

BUILD_IN	=	src/build_in/cd.c \
				src/build_in/echo.c \
				src/build_in/env.c \
				src/build_in/exit.c \
				src/build_in/export_parser.c \
				src/build_in/export_utils.c \
				src/build_in/export.c \
				src/build_in/pwd.c \
				src/build_in/unset.c

EXECUTION	=	src/execution/check_the_acces.c \
				src/execution/execute_build_in.c \
				src/execution/execute_external.c \
				src/execution/execute.c \
				src/execution/get_path.c \
				src/execution/utilities.c

HERE_DOC	=	src/here_doc/expand.c \
				src/here_doc/here_doc_utils.c \
				src/here_doc/here_doc.c

INIT		=	src/init/init_data.c \
				src/init/init_envp.c \
				src/init/init_cmd.c

LEXER		=	src/lexer/calc_token_len.c \
				src/lexer/copy_token.c \
				src/lexer/identify_tokens.c \
				src/lexer/lexer.c \
				src/lexer/token_parser.c \
				src/lexer/utils.c 

PARSER		=	src/parser/check_token_syntax.c \
				src/parser/counting.c \
				src/parser/handle_redirection.c \
				src/parser/parser.c \
				src/parser/prepar_cmd.c

UTILS		=	src/utils/cleanup.c \
				src/utils/error.c \
				src/utils/exit_all.c \
				src/utils/free_all.c \
				src/utils/get_char.c \
				src/utils/get_int.c \
				src/utils/utilities_.c \

BASICS		=	src/header.c \
				src/main.c \
				src/signals_execution.c \
				src/signals.c

SRCS		=	${BUILD_IN} \
				${EXECUTION} \
				${HERE_DOC} \
				${INIT} \
				${LEXER} \
				${PARSER} \
				${UTILS} \
				${BASICS}

OBJS		=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

INCLUDES	=	include/

PATH_LIBFT	=	$(INCLUDES)/my_lib

MY_LIB		=	$(PATH_LIBFT)/my_lib.a

LIBFT		=	$(PATH_LIBFT)/$(MY_LIB)

VPATH =	src/build_in:src/execution:src/here_doc:src/init:src/lexer:src/parser:src/utils:src

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	# Linux flags
	#@$(CC) $(CFLAGS) -I $(INCLUDES) -c $< -o $@
	# Mac flags
	@$(CC) $(CFLAGS) -I $(INCLUDES) -I$(READLINE_DIR)/include -c $< -o $@
	@echo "Compiled: $<"

$(NAME): $(MY_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MY_LIB) ${LDFLAGS} -o $(NAME)

$(MY_LIB):
	make -C $(PATH_LIBFT)

clean:
	$(RM) -r $(OBJ_DIR)
	make clean -C $(PATH_LIBFT)

fclean: clean
	$(RM) $(NAME)
	make fclean -C $(PATH_LIBFT)

re: fclean all

header: $(MY_LIB)
	$(CC) $(CFLAGS) src/header.c $(MY_LIB) -I $(INCLUDES) -o header

.PHONY: all clean fclean re test_parser test_lexer_parser test_all clean_tests
