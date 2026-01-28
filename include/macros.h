/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 02:22:22 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/09 07:51:54 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H

extern int	g_exit_status;

/* ========================================================================== */
/*                              COLOR MACROS                                  */
/* ========================================================================== */

# define RESET		"\033[0m"
# define VIOLET_BOLD	"\033[1;35m"
# define YELLOW_BOLD	"\033[1;33m"
# define BLUE		"\033[0;34m"
# define RED_BOLD	"\033[1;31m"
# define GREEN_BOLD	"\033[1;32m"
/* ========================================================================== */

# ifndef DELAY_TIME
#  define DELAY_TIME 1600000
# endif

# ifndef DELAY_DOT
#  define DELAY_DOT 1000000
# endif

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# ifndef MAIN_SIGNAL
#  define MAIN_SIGNAL 1
# endif

# ifndef CHILD_SIGNAL
#  define CHILD_SIGNAL 2
# endif

# ifndef HEREDOC_CHILD
#  define HEREDOC_CHILD 3
# endif

# ifndef HEREDOC_PARENT
#  define HEREDOC_PARENT 4
# endif

typedef enum e_tok_type
{
	COMMAND,
	FLAG,
	STRING,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
	T_EOF,
	STDIN,
	STDOUT,
	QUOTE,
	NONE,
}				t_tok_type;

#endif