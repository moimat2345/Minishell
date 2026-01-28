/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 02:24:12 by matnusko          #+#    #+#             */
/*   Updated: 2026/01/09 07:52:21 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "macros.h"

struct		s_tokens;
struct		s_data;

typedef struct s_envp
{
	char				*name;
	char				*value;
	struct s_envp		*next;
	struct s_envp		*prev;
}				t_envp;
typedef struct s_tokens
{
	int				position;
	char			*value;
	t_tok_type		type;
	bool			simple_quote;
	bool			was_quoted;
	struct s_tokens	*next;
	struct s_tokens	*prev;
	t_envp			*env_var;
	struct s_data	*data;
}				t_tokens;

typedef struct s_cmd
{
	char				**args;
	int					args_count;
	int					is_built_in;
	int					has_redir_error;
	char				*input_file;
	char				*output_file;
	char				*delimiter;
	int					input_fd;
	int					output_fd;
	int					append;
	int					here_doc;
	int					heredoc_expand;
	struct s_tokens		*tokens_start;
	struct s_cmd		*next;
	struct s_cmd		*prev;
}				t_cmd;

typedef struct s_data
{
	char			**here_doc_temp_files;
	char			**envp;
	char			*input;
	int				num_cmds;
	int				exit_code;
	int				input_fd;
	int				output_fd;
	int				append;
	int				here_doc;
	char			*input_file;
	char			*output_file;
	char			*delimiter;
	t_tokens		**tokens;
	t_envp			*env_var;
	t_cmd			*cmds;
}				t_data;

#endif