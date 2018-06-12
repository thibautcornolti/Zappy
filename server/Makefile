##
## EPITECH PROJECT, 2018
## zappy
## File description:
## Makefile
##

MAKEPOLL	=	make --no-print-directory -C lib/poll
MAKESOCKET	=	make --no-print-directory -C lib/socket

CC		=	gcc

NAME		=	zappy_server

SRCDIR		=	src
OBJDIR		=	build

FILES		=	main			\
			task_assigner		\
			commands/cmd_msz	\
			commands/cmd_bct	\
			commands/cmd_mct	\
			commands/cmd_tna	\
			commands/cmd_ppo	\
			commands/cmd_plv	\
			commands/cmd_pin	\
			commands/cmd_sgt	\
			commands/cmd_sst

SORTEDFILES	=	$(sort $(FILES))
SRCS		=	$(SORTEDFILES:%=$(SRCDIR)/%.c)
OBJS		=	$(SORTEDFILES:%=$(OBJDIR)/%.o)

CFLAGS 		=	-Ilib -Iinclude -W -Wall -Wextra -Wunused -Wuninitialized
LDFLAGS		=	-Llib/socket -lsocket -Llib/poll -lpoll -Llib/list -llist

DIRS		=	commands

SORTEDDIRS	=	$(sort $(DIRS))
SRCDIRS		=	$(SORTEDDIRS:%=$(SRCDIR)/%)
OBJDIRS		=	$(SORTEDDIRS:%=$(OBJDIR)/%)

REDDARK 	=       \033[31;2m
RED     	=       \033[31;1m
GREEN   	=       \033[32;1m
YEL     	=       \033[33;1m
BLUE    	=       \033[34;1m
PINK    	=       \033[35;1m
CYAN    	=       \033[36;1m
RES     	=       \033[0m

all:	$(NAME)

$(NAME):	$(OBJDIR) $(OBJDIRS) $(OBJS)
		@$(MAKEPOLL)
		@$(MAKESOCKET)
		@echo -e "[$(RED)LINKING$(RES)] $(YEL)$(OBJDIR)/server/*.o$(RES) $(BLUE)=>$(RES) $(YEL)$(NAME)$(RES)"
		@gcc -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	@$(MAKEPOLL) clean
	@$(MAKESOCKET) clean
	@rm -rf $(OBJDIR)

fclean:	clean
	@$(MAKEPOLL) fclean
	@$(MAKESOCKET) fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	@echo -e "[$(RED)COMPILE$(RES)] $(YEL)$<$(RES) $(BLUE)=>$(RES) $(YEL)$@$(RES)"
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	@echo -e "[$(RED)MKDIR$(RES)] $(YEL)build$(RES)"
	@mkdir -p $@

$(OBJDIR)/%:
	@echo -e "[$(RED)MKDIR$(RES)] $(YEL)$<$(RES)"
	@mkdir -p $@
