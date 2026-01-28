/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   library.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 02:20:56 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/09 02:21:23 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBRARY_H
# define LIBRARY_H

/* printf(), perror() */
# include <stdio.h>

/* readline(), add_history() */
# include <readline/readline.h>
# include <readline/history.h>

/* malloc(), free(), exit() */
# include <stdlib.h>

/* write(), access(), dup2(), execve(), fork(), pipe() */
# include <unistd.h>

/* open(), O_* flags for redirections */
# include <fcntl.h>

/* strerror(), strlen(), strcmp(), etc. */
# include <string.h>

/* errno global variable */
# include <errno.h>

/* wait(), waitpid(), wait3(), wait4() */
# include <sys/wait.h>

/* kill(), sigaction(), signals handling */
# include <signal.h>

/* stat(), lstat(), fstat() */
# include <sys/stat.h>

/* terminal control: ioctl(), ttyname(), etc. */
# include <sys/ioctl.h>

/* opendir(), readdir(), closedir() */
# include <dirent.h>

/* termcap functions: tgetent(), tputs(), tgoto(), etc. */
# include <termcap.h>

/* termios: tcsetattr(), tcgetattr() */
# include <termios.h>

/* bool, true, false */
# include <stdbool.h>

#endif