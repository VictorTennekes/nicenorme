/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: vtenneke <vtenneke@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/28 20:03:43 by vtenneke      #+#    #+#                 */
/*   Updated: 2020/10/29 14:55:59 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
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

int main(int ac, char **av) {
	string	input;
	string	id;
	file	current_file;
	int	std_fd_saved[2];
	std_fd_saved[0] = dup(STDIN_FILENO);
	std_fd_saved[1] = dup(STDOUT_FILENO);
	int	norme_pipe[2];

	if (pipe(norme_pipe) == -1)
		exit (1);
	if (dup2(norme_pipe[0], STDIN_FILENO) == -1)
		exit (1);
	int pid = fork();
	if (pid == 0)
	{
		if (dup2(norme_pipe[1], STDOUT_FILENO) == -1)
			exit (1);
		int		tmp_pipe[2];
		int		saved_io[2];
		if (pipe(tmp_pipe) == -1)
			exit (1);
		saved_io[0] = dup(STDIN_FILENO);
		saved_io[1] = dup(STDOUT_FILENO);
		string norm_path;
		if (dup2(tmp_pipe[1], STDOUT_FILENO) == -1)
			exit (1);
		if (dup2(tmp_pipe[0], STDIN_FILENO) == -1)
			exit (1);
		system("which norminette");
		cin >> norm_path;
		if (dup2(saved_io[1], STDOUT_FILENO) == -1)
			exit (1);
		if (dup2(saved_io[0], STDIN_FILENO) == -1)
			exit (1);
		close(saved_io[0]);
		close(saved_io[1]);
		close(tmp_pipe[0]);
		close(tmp_pipe[1]);
		cerr << "Norme path bitches: " << norm_path << endl;
		if (execlp(norm_path.c_str(), "norminette", NULL) == -1)
			exit (1);
		exit (0);
	}
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
		if (std::cin.eof())
			break ;
	}
	cerr << "YEET" << endl;
	close(norme_pipe[0]);
	close(norme_pipe[1]);
	close(std_fd_saved[0]);
	close(std_fd_saved[1]);
	exit(0);
	waitpid(pid, NULL, 0);
	
//	if (dup2(std_fd_saved[0], STDIN_FILENO) == -1)
//		exit (1);
	current_file.print_errors(true);
}
