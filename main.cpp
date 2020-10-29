/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: vtenneke <vtenneke@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/28 20:03:43 by vtenneke      #+#    #+#                 */
/*   Updated: 2020/10/29 17:08:49 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "normeError.hpp"
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

message	parseError(string input) {
	message error;
	
	size_t delim = input.find_first_of("(", 6);
	if (delim == string::npos) {
		delim = input.find_first_not_of(" ", 6);
		error.description = input.substr(delim, input.size() - delim);
		return (error);
	}
	size_t delim_sec = input.find_first_of("),", delim);
	if (input[delim_sec] == ')') {
		error.line = input.substr(delim + 6, delim_sec - (delim + 6));
	} else if (input[delim_sec]  == ',') {
		error.line = input.substr(delim + 6, delim_sec - (delim + 6));
		delim = input.find_first_of(")", delim_sec);
		error.column = input.substr(delim_sec + 6, delim - (delim_sec + 6));
		delim_sec = delim;
	}
	error.description = input.substr(delim_sec + 3, input.size() - delim_sec + 3);
	return (error);
}

char	*strdup(char *orig)
{
	size_t len = 0;
	for (; orig && orig[len]; len++) {}
	char	*dup = (char *)malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	for (size_t i = 0; i < len; i++)
		dup[i] = orig[i];
	dup[len] = '\0';
	return (dup);
}

char	**create_args(int ac, char **av)
{
	char	**args = (char **)malloc(sizeof(char *) * (ac + 2));
	if (!args)
		return (NULL);
	args[0] = strdup((char *)("norminette"));
	if (!args[0])
		return (NULL);
	for (size_t i = 1; i < ac + 1; i++)
	{
		args[i] = strdup(av[i - 1]);
		if (!args[i])
			return (NULL);
	}
	args[ac + 1] = NULL;
	return (args);
}

void	print_args(char **av)
{
	for (size_t i = 0; av[i]; i++)
		dprintf(2, "ARG[%ld] = %s\n", i, av[i]);
}

void	destroy_args(char **av)
{
	for (size_t i = 0; av[i]; i++)
	{
		free(av[i]);
	}
	free(av);
}

int main(int ac, char **av) {
	string			input;
	string			id;
	file			current_file;
	extern char 	**environ;
	char			**args;
	int				std_fd_saved[2] = {dup(STDIN_FILENO), dup(STDOUT_FILENO)};
	int				norme_pipe[2];

	args = create_args(ac - 1, &av[1]);
	if (!args)
		return (1);
//	print_args(args);
	if (pipe(norme_pipe) == -1 || dup2(norme_pipe[0], STDIN_FILENO) == -1)
		exit (1);
	int pid = fork();
	if (pid == 0)
	{
		if (dup2(norme_pipe[1], STDOUT_FILENO) == -1)
			exit (1);
		int		tmp_pipe[2];
		int		saved_io[2] = {dup(STDIN_FILENO), dup(STDOUT_FILENO)};
		if (pipe(tmp_pipe) == -1)
			exit (1);
		if (dup2(tmp_pipe[1], STDOUT_FILENO) == -1 || dup2(tmp_pipe[0], STDIN_FILENO) == -1)
			exit (1);
		system("which norminette");
		string norm_path;
		cin >> norm_path;
		if (dup2(saved_io[1], STDOUT_FILENO) == -1 || dup2(saved_io[0], STDIN_FILENO) == -1)
			exit (1);
		close(saved_io[0]);
		close(saved_io[1]);
		close(tmp_pipe[0]);
		close(tmp_pipe[1]);
		if (execve(norm_path.c_str(), args, environ) == -1)
			exit (1);
		exit (0);
	}
	destroy_args(args);
	close(norme_pipe[1]);
	while (getline(std::cin, input)) {
		message	current_error;

		size_t delim = input.find_first_of(": ", 0);
		id = input.substr(0, delim);
		if (id == "Norme") {
			if (current_file.name.size() > 0 && (current_file.error.size() > 0 || current_file.warning.size() > 0)) {
				current_file.print_errors(false);
			}
			current_file.error.clear();
			current_file.warning.clear();
			current_file.name = input.substr(7, input.size() - 7);
		} else if (id == "Warning") {
			current_error.description = input.substr(9, input.size() - 9);
			current_file.warning.push_back(current_error);
		} else if (id == "Error") {
			current_file.error.push_back(parseError(input));
		}

	}
	close(norme_pipe[0]);
	close(std_fd_saved[0]);
	close(std_fd_saved[1]);
	waitpid(pid, NULL, 0);
	current_file.print_errors(true);
}
