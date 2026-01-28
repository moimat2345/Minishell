/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:42:34 by mateon            #+#    #+#             */
/*   Updated: 2026/01/14 13:45:48 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//*************************************************************************/
//*    ███╗   ███╗██╗███╗   ██╗██╗    ██╗  ██╗███████╗██╗     ██╗         */
//*    ████╗ ████║██║████╗  ██║██║    ██║  ██║██╔════╝██║     ██║         */
//*    ██╔████╔██║██║██╔██╗ ██║██║    ███████║█████╗  ██║     ██║         */
//*    ██║╚██╔╝██║██║██║╚██╗██║██║    ██╔══██║██╔══╝  ██║     ██║         */
//*    ██║ ╚═╝ ██║██║██║ ╚████║██║    ██║  ██║███████╗███████╗███████╗    */
//*    ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝    ╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝    */
//*************************************************************************/

#ifndef MINISHELL_H
# define MINISHELL_H

# include "my_lib/include/libft.h"
# include "library.h"
# include "struct.h"

//*********************************************************** */
//*                     BUILD IN FUNCTIONS                    */
//*********************************************************** */

//>>>>>>>>>>>>>>>>>>>>> cd.c <<<<<<<<<<<<<<<<<<<<//
void		update_pwd_envvar(t_data *data);
void		update_oldpwd_envvar(t_data *data, char *oldpwd);
void		build_in_cd(t_data *data, char **args);

//>>>>>>>>>>>>>>>>>>>>> echo.c <<<<<<<<<<<<<<<<<<<<//
int			check_the_flag(char *arg);	
void		build_in_echo(t_data *data, char **args);

//>>>>>>>>>>>>>>>>>>>>> env.c <<<<<<<<<<<<<<<<<<<<//
void		build_in_env(t_data *data, char **args);

//>>>>>>>>>>>>>>>>>>>>> exit.c <<<<<<<<<<<<<<<<<<<<//
int			parser_exit_argument(char **args, t_data **data, int args_c);
void		exit_exit(char *message, int exit_code, t_data **data);
void		build_in_exit(t_data *data, char **args);

//>>>>>>>>>>>>>>>>>>>>> export.c <<<<<<<<<<<<<<<<<<<<//
int			is_valid_identifier(char *name);
int			parse_with_equal(char *arg, char **name,
				char **value, char *equal_sign);
int			parser_export_argument(char *arg, char **name,
				char **value, int *has_equal);
void		check_which_add(t_data *data, char *name,
				char *value, int has_equal);
int			count_env(t_data *data);
void		sort_envp_array(t_envp **env_array, int size);
void		print_export(t_envp *env_ar);
void		print_sorted_envp(t_data *data);
t_envp		*find_node_in_envp(t_envp *env, char *name);
void		update_envp_value(t_envp *node, char *new_value);
void		add_in_envp_struct(t_data *data, char *name, char *value);
int			process_export_arg(t_data *data, char *arg);
void		build_in_export(t_data *data, char **args);

//>>>>>>>>>>>>>>>>>>>>> pwd.c <<<<<<<<<<<<<<<<<<<<//
void		build_in_pwd(t_data *data);

//>>>>>>>>>>>>>>>>>>>>> unset.c <<<<<<<<<<<<<<<<<<<<//
void		remove_from_struct(t_data *data, t_envp *current);
void		build_in_unset(t_data *data, char **args);

//*********************************************************** */
//*                     EXECUTION FUNCTIONS                   */
//*********************************************************** */

//>>>>>>>>>>>>>>>>>>>>> check_the_acces.c <<<<<<<<<<<<<<<<<<<<//
int			check_output_dir(char *filename, t_data **data);
int			check_file_access(char *filename, int is_input, t_data **data);
void		open_input_files(t_cmd *cmd, t_tokens **current, t_data *data);
void		open_output_files(t_cmd *cmd, t_tokens **current, t_data *data);
void		open_cmd_files(t_cmd *cmd, t_data *data);

//>>>>>>>>>>>>>>>>>>>>> execute_build_in.c <<<<<<<<<<<<<<<<<<<<//
void		execute_build_in(t_cmd *cmd, t_data *data);
void		found_built_in_commands(t_data *data);

//>>>>>>>>>>>>>>>>>>>>> execute_external.c <<<<<<<<<<<<<<<<<<<<//
void		error_command_execve(char *cmd, t_data **data);
int			count_envp(t_envp *envp);
char		*create_env_string(t_envp *current);
char		**convert_envp_to_strs(t_envp *envp);
void		execute_external_function(t_cmd *cmd, t_data *data, t_envp **envp);

//>>>>>>>>>>>>>>>>>>>>> execute.c <<<<<<<<<<<<<<<<<<<<//
void		duplicate_fd(int (*pipefd)[2], t_cmd *cmd, int n, int i);
void		execute_child(t_cmd *cmd, t_data *data, t_envp **envp,
				int (*pipefd)[2]);
void		check_if_malloc_failed_plus_create_pipe(int (*pipefd)[2],
				t_data *data, pid_t *pids);
int			process(t_data *data, t_cmd *cmd_original, t_envp **envp);
void		execution(t_data *data, t_envp **envp);

//>>>>>>>>>>>>>>>>>>>>> get_path.c <<<<<<<<<<<<<<<<<<<<//
char		*ft_strjoin_with_slash(char *s1, char *s2);
char		*check_absolute_path(char *expanded_cmd, t_data *data);
char		*search_in_paths(char **paths, char *cmd, t_data *data);
char		*find_command_path(char *cmd, t_envp **envp, t_data *data);
char		*find_cmd_path(char **paths, char *cmd);

//>>>>>>>>>>>>>>>>>>>>> utilities.c <<<<<<<<<<<<<<<<<<<<//
char		*expand_tilde(char *cmd, t_envp **envp);
int			handle_child_status(int status);
void		wait_all_children(int n, t_data *data, pid_t *pids);
void		create_pipes(int (*pipefd)[2], int n);
void		close_all_pipes(int (*pipefd)[2], int n);

//*********************************************************** */
//*                    HERE DOC FUNCTIONS                     */
//*********************************************************** */

//>>>>>>>>>>>>>>>>>>>>> expand.c <<<<<<<<<<<<<<<<<<<<//
void		expand_heredoc_var(char *line, char *dest, int *ij, t_data *data);
int			calc_expanded_len(char *line, t_data *data);
void		expand_exit_code_to_dest(char *exp, int *j, t_data *data, int *i);
char		*expand_heredoc_line(char *line, t_data *data);

//>>>>>>>>>>>>>>>>>>>> here_doc_utils.c <<<<<<<<<<<<<<<<<<<<//
void		copy_var_to_dest(char *dest, char *var_value, int *j);
int			add_var_len_to_total(char *line, int *i, t_envp *env);
char		*build_temp_name(int num);
char		*make_temp_filename(t_data *data);
void		write_heredoc_line(t_cmd *cmd, t_data *data, char *line);

//>>>>>>>>>>>>>>>>>>>>> here_doc.c <<<<<<<<<<<<<<<<<<<<//
void		heredoc_child_process(t_cmd *cmd, t_data *data, int store_fd);
void		heredoc_parent_process(t_cmd *cmd, t_data *data,
				char *temp, int store_fd);
void		handle_here_doc(t_cmd *cmd, t_data *data);
int			process_here_docs(t_data *data);

//*********************************************************** */
//*                    INIT FUNCTIONS                         */
//*********************************************************** */

//>>>>>>>>>>>>>>>>>>>>> init_cmd.c <<<<<<<<<<<<<<<<<<<<//
void		init_new_cmd(t_cmd **new_cmd, t_data **data);

//>>>>>>>>>>>>>>>>>>>>> init_data.c <<<<<<<<<<<<<<<<<<<<//
void		init_data(t_data *data, char **environ);

//>>>>>>>>>>>>>>>>>>>>> init_envp.c <<<<<<<<<<<<<<<<<<<<//
void		init_first_or_new_envp(t_data *data, t_envp *new_var);
void		increment_shlvl(t_data *data);
void		init_envp(char **envp, t_data *data, int i);

//*********************************************************** */
//*                    LEXER FUNCTIONS                        */
//*********************************************************** */

//>>>>>>>>>>>>>>>> calc_token_len.c <<<<<<<<<<<<<<<<<<<<//
int			clac_signle_quote_len(char *s, int *i);
int			calc_var_len(char *s, int *i, t_envp *env_var);
int			calc_double_quote_len(char *s, int *i, t_envp *env_var);
int			calc_token_len_special_var_len(t_tokens *token,
				int *i, t_envp *env_var);
int			calc_token_len(t_tokens *token);

//>>>>>>>>>>>>>>>> copy_token.c <<<<<<<<<<<<<<<<<<<<//
void		copy_exp_var(t_tokens *token, char *dest, int *i, int *j);
void		check_if_special_var(t_tokens *token,
				char *dest, int *i, int *j);
int			copy_single_quote(char *src, char *dest, int *i, int *j);
int			copy_double_quote(t_tokens *token, char *dest, int *i, int *j);
void		copy_token_value(t_tokens *token, t_data *data);

//>>>>>>>>>>>>>>>> identify_tokens.c <<<<<<<<<<<<<<<<<<<<//
t_tokens	*cut_lexeur(char *s);
int			is_redirector(t_tok_type type);
t_tok_type	get_opperator_type(char *value);
int			in_or_out(t_tokens *current);
void		identify_tokens(t_tokens *token);

//>>>>>>>>>>>>>>>> lexer.c <<<<<<<<<<<<<<<<<<<<//
void		set_newtoken(t_tokens *new_token, char *value, bool sp_quotes);
void		add_token(t_tokens **rendu, char *value, bool sp_quotes);
void		lexer_main(t_data *data);
void		quotes_handler(t_tokens *token, char *new_value, int *i, int *j);

//>>>>>>>>>>>>>>>> token_parser.c <<<<<<<<<<<<<<<<<<<<//
void		handle_quotes_in_token(t_tokens *token, int j);
void		handle_quotes(t_tokens *tokens);
void		unlink_token(t_tokens **tokens, t_tokens *node, t_tokens *prev);
void		remove_empty_tokens(t_tokens **tokens);
t_tokens	*token_parser(t_tokens *tokens, t_data *data);

//>>>>>>>>>>>>>>>> utils.c <<<<<<<<<<<<<<<<<<<<//
int			is_a_dollar_char(char c);
int			is(char *what, char c);
void		handle_specials(char *s, int *i, int *start);
void		handle_token(char *s, int *i, int *start);
char		*find_in_envp(char *the_dollar, t_envp *env_var);

//*********************************************************** */
//*                    PARSER FUNCTIONS                       */
//*********************************************************** */

//>>>>>>>>>>>>>>>>>>>>> check_token_syntax.c <<<<<<<<<<<<<<<<<<<<//
int			check_pipe_syntax(t_tokens *token);
int			check_redirection_syntax(t_tokens *token);
int			is_redirection_token(t_tok_type type);
int			check_type_syntax(t_tokens *token, t_data *data);
int			check_tokens_syntax(t_data *data);

//>>>>>>>>>>>>>>>>>>>>> counting.c <<<<<<<<<<<<<<<<<<<<//
int			count_commands(t_tokens **tokens);
int			count_args(t_tokens *token);

//>>>>>>>>>>>>>>>>>>>> handle_redirection.c <<<<<<<<<<<<<<<<<<<<//
void		handle_redirect_in(t_cmd *cmd, t_tokens *token);
void		handle_redirect_out(t_cmd *cmd, t_tokens *token);
void		extract_the_redirection(t_tokens **token,
				t_data **data_in, t_cmd *cmd);

//>>>>>>>>>>>>>>>>>>>>> parser.c <<<<<<<<<<<<<<<<<<<<//
int			parsing(t_data *data);

//>>>>>>>>>>>>>>>>>>>>> prepar_cmd.c <<<<<<<<<<<<<<<<<<<<//
void		create_cmds_list(t_cmd **cmds, int total_cmds, t_data **data);
t_tokens	*next_token(t_tokens *token);
char		**fill_the_args(t_tokens *token, int args_count);
char		**extract_args(t_tokens **token);
void		cut_to_cmds(t_data *data);

//*********************************************************** */
//*                    UTILS FUNCTIONS                        */
//*********************************************************** */

//>>>>>>>>>>>>>>>>>>>>>> cleanup.c <<<<<<<<<<<<<<<<<<<<//
void		clean_token(t_data *data);
void		clean_files(t_data *data);
void		clean_fds(t_data *data);
void		delete_temp_files(char **temp_files);
void		cleanup_for_next_command(t_data *data);

//>>>>>>>>>>>>>>>>>>>>>>> error.c <<<<<<<<<<<<<<<<<<<<//
int			error_syntax(t_data *data, char *value);
void		error_here_doc(t_data *data, char *message);
void		error_minishell(char *message, int exit_code, t_data **data);
void		file_access_error(char *filename, t_data **data);
void		print_export_error(char *arg);

//>>>>>>>>>>>>>>>>>>>>>> exit_all.c <<<<<<<<<<<<<<<<<<<<//
void		exit_with_error(char *message, int exit_code, t_data **data);
void		handle_arg_error(char **args, t_tokens *token, int i);
void		error_unset_path(char *cmd, t_data **data);

//>>>>>>>>>>>>>>>>>>>>>>> free_all.c <<<<<<<<<<<<<<<<<<<<//
void		free_strs(char **strs);
void		free_envp(t_envp **env_var);
void		free_cmds(t_cmd **cmds);

//>>>>>>>>>>>>>>>>>>>>>>> get_char.c <<<<<<<<<<<<<<<<<<<<//
char		*get_directory_path(char *filename);
char		*handle_tilde(char **args, char **path, t_data *data);
char		*get_cd_path(t_data *data, char **args);
char		*get_var_name(char *s, int i, int var_len);
char		**get_path(t_envp **env);

//>>>>>>>>>>>>>>>>>><<<<< get_int.c <<<<<<<<<<<<<<<<<<<<//
int			get_var_name_len(char *s, int i);
int			get_cmd_index(t_cmd *start, t_cmd *target);

//>>>>>>>>>>>>>>>>>>>>>>> utilities_.c <<<<<<<<<<<<<<<<<<<<//
void		check_existence_of_fd(int *fd);
void		solo_build_in_execution_and_handling_of_fd(t_data **data,
				t_cmd **cmd);

//*********************************************************** */
//*                    SIGNALS FUNCTIONS                      */
//*********************************************************** */

//>>>>>>>>>>>>>>>>>>>>>> signals_execution.c <<<<<<<<<<<<<<<<<<<<//
void		ctrl_c(int sig);
void		ctrl_backslash(int sig);
void		reset_prompt(int sig);
void		heredoc_child(int sig);
void		heredoc_parent(int sig);

//>>>>>>>>>>>>>>>>>>>>>> signals.c <<<<<<<<<<<<<<<<<<<<//
void		set_signal(int signal_type);

//*********************************************************** */
//*                    MAIN FUNCTION                          */
//*********************************************************** */

//>>>>>>>>>>>>>>>>>>>>>> header.c <<<<<<<<<<<<<<<<<<<<//
void		exec_clear(void);
void		print_delay(char *s, long time);
void		ascii(void);
void		loading_bar(void);
void		header(void);

//>>>>>>>>>>>>>>>>>>>>>> main.c <<<<<<<<<<<<<<<<<<<<//
void		main_process(t_data *data, int exit_code);
int			main(int argc, char **argv, char **envp);

//*************************************************************************/
//*    ███╗   ███╗██╗███╗   ██╗██╗    ██╗  ██╗███████╗██╗     ██╗         */
//*    ████╗ ████║██║████╗  ██║██║    ██║  ██║██╔════╝██║     ██║         */
//*    ██╔████╔██║██║██╔██╗ ██║██║    ███████║█████╗  ██║     ██║         */
//*    ██║╚██╔╝██║██║██║╚██╗██║██║    ██╔══██║██╔══╝  ██║     ██║         */
//*    ██║ ╚═╝ ██║██║██║ ╚████║██║    ██║  ██║███████╗███████╗███████╗    */
//*    ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝    ╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝    */
//*************************************************************************/

#endif
