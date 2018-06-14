##
## EPITECH PROJECT, 2018
## zappy
## File description:
## Makefile
##

MAKESTR		=	make --no-print-directory -C lib/str
MAKEPOLL	=	make --no-print-directory -C lib/poll
MAKELIST	=	make --no-print-directory -C lib/list
MAKESOCKET	=	make --no-print-directory -C lib/socket

CC		=	gcc

NAME		=	zappy_server

SRCDIR		=	src
OBJDIR		=	build

FILES		=	args				\
			assigner			\
			main				\
			parser				\
			t_manager			\
			t_tools				\
			map				\
			exec				\
			resource_gen			\
			commands/cmd_broadcast		\
			commands/cmd_connect_nbr	\
			commands/cmd_eject		\
			commands/cmd_fork		\
			commands/cmd_forward		\
			commands/cmd_incantation	\
			commands/cmd_inventory		\
			commands/cmd_left		\
			commands/cmd_look		\
			commands/cmd_right		\
			commands/cmd_set		\
			commands/cmd_take		\
			admin/adm_teams			\
			admin/adm_spawn			\
			admin/adm_tickrate		\
			admin/adm_move			\
			admin/adm_cast			\
			admin/adm_killall		\
			arguments/parse_clients		\
			arguments/parse_freq		\
			arguments/parse_height		\
			arguments/parse_help		\
			arguments/parse_names		\
			arguments/parse_port		\
			arguments/parse_width

SORTEDFILES	=	$(sort $(FILES))
SRCS		=	$(SORTEDFILES:%=$(SRCDIR)/%.c)
OBJS		=	$(SORTEDFILES:%=$(OBJDIR)/%.o)

CFLAGS 		=	-Ilib -Iinclude -W -Wall -Wextra -Wunused -Wuninitialized
LDFLAGS		=	-Llib/socket -lsocket -Llib/poll -lpoll -Llib/list -llist -Llib/str -lstr -lm

DIRS		=	commands		\
			arguments

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
		@$(MAKESTR)
		@$(MAKEPOLL)
		@$(MAKELIST)
		@$(MAKESOCKET)
		@echo -e "[$(RED)LINKING$(RES)] $(YEL)$(OBJDIR)/server/*.o$(RES) $(BLUE)=>$(RES) $(YEL)$(NAME)$(RES)"
		@gcc -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	@$(MAKESTR) clean
	@$(MAKEPOLL) clean
	@$(MAKELIST) clean
	@$(MAKESOCKET) clean
	@rm -rf $(OBJDIR)

fclean:	clean
	@$(MAKESTR) fclean
	@$(MAKEPOLL) fclean
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
	@echo -e "[$(RED)MKDIR$(RES)] $(YEL)$@$(RES)"
	@mkdir -p $@
