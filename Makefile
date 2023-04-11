# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omansour <omansour@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/26 20:05:18 by omansour          #+#    #+#              #
#    Updated: 2023/04/10 22:22:42 by omansour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS	=	philo.c

OBJS	=	${SRCS:.c=.o}

NAME	=	philo

CFLAGS	=	-Wall -Wextra -Werror #-fsanitize=address

CC		=	cc

RM		=	rm -f

all : ${NAME}

${NAME} : ${OBJS}
		${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean :
		${RM} ${OBJS}

fclean : clean
		${RM} ${NAME}

re : fclean all
