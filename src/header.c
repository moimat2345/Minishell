/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 16:57:54 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/09 08:08:28 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* ========================================================================== */
/*                           UTILITY FUNCTIONS                                */
/* ========================================================================== */

void	exec_clear(void)
{
	char		*argv[2];
	pid_t		pid;
	extern char	**environ;

	argv[0] = "clear";
	argv[1] = NULL;
	pid = fork();
	if (pid < 0)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		execve("/usr/bin/clear", argv, environ);
		write(2, "execve error\n", 13);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	waitpid(pid, NULL, 0);
}

void	print_delay(char *s, long time)
{
	long	delay;

	while (*s)
	{
		delay = 0;
		while (delay < time)
			delay++;
		ft_putchar(*s);
		s++;
	}
}

/* ========================================================================== */
/*                              ASCII ART                                     */
/* ========================================================================== */

void	ascii(void)
{
	print_delay(RED_BOLD"\
//**************************************************************\
*//\n", DELAY_TIME);
	print_delay("\
//*                                                             \
*//\n", DELAY_TIME);
	print_delay("\
//* ███╗   ███╗██╗███╗   ██╗██╗██╗  ██╗███████╗██╗     ██╗      \
*//\n", DELAY_TIME);
	print_delay("\
//* ████╗ ████║██║████╗  ██║██║██║  ██║██╔════╝██║     ██║      \
*//\n", DELAY_TIME);
	print_delay("\
//* ██╔████╔██║██║██╔██╗ ██║██║███████║█████╗  ██║     ██║      \
*//\n", DELAY_TIME);
	print_delay("\
//* ██║╚██╔╝██║██║██║╚██╗██║██║██╔══██║██╔══╝  ██║     ██║      \
*//\n", DELAY_TIME);
	print_delay("\
//* ██║ ╚═╝ ██║██║██║ ╚████║██║██║  ██║███████╗███████╗███████╗ \
*//\n", DELAY_TIME);
	print_delay("\
//* ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝ \
*//\n", DELAY_TIME);
	print_delay("\
//*                                                             \
*//\n", DELAY_TIME);
	print_delay("\
//*                By matnusko & nkuydin                        \
*//\n", DELAY_TIME);
	print_delay("\
//*                                                             \
*//\n", DELAY_TIME);
	print_delay("\
//**************************************************************\
*//\n"RESET, DELAY_TIME);
}

/* ========================================================================== */
/*                          LOADING ANIMATION                                 */
/* ========================================================================== */

void	loading_bar(void)
{
	ft_putstr("\n  Loading : ");
	print_delay(BLUE"█████", DELAY_DOT * 5);
	print_delay("██", DELAY_DOT * 20);
	print_delay("███", DELAY_DOT * 8);
	print_delay("████", DELAY_DOT * 15);
	print_delay("██", DELAY_DOT * 10);
	print_delay("███████", DELAY_DOT * 6);
	print_delay("██████", DELAY_DOT * 12);
	print_delay("███"RESET, DELAY_DOT * 3);
	ft_putstr(GREEN_BOLD" ✓"RESET"\n\n");
}

/* ========================================================================== */
/*                            MAIN HEADER                                     */
/* ========================================================================== */

void	header(void)
{
	long	delay;

	delay = 0;
	exec_clear();
	loading_bar();
	while (delay < DELAY_TIME * 2)
		delay++;
	exec_clear();
	ascii();
	delay = 0;
	while (delay < DELAY_TIME * 3)
		delay++;
}
