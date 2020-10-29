/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: vtenneke <vtenneke@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/28 20:03:43 by vtenneke      #+#    #+#                 */
/*   Updated: 2020/10/28 20:03:43 by vtenneke      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include "normeError.hpp"

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

	string	kernel;
	system("uname");
	cin >> kernel;
	if (system("which norminette > /dev/null") == 1) {
		cerr << "Please install norminette before using nicenorme" << endl;
		return(1);
	}
	int pid = fork();
	if (pid == 0) {
		string norm_path;
		system("which norminette");
		cin >> norm_path;
		cerr << "Norme path bitches: " << norm_path << endl;
		execlp(norm_path.c_str(), "norminette", NULL);
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
	}
	current_file.print_errors(true);
}