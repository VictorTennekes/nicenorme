/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   normeError.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: vtenneke <vtenneke@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/28 19:44:24 by vtenneke      #+#    #+#                 */
/*   Updated: 2020/10/28 19:44:24 by vtenneke      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

class	message {

	public:
		std::string				description;
		std::string				line;
		std::string				column;

		message() {
			line = "";
			column = "";
		}
};

class	file {
	public:
		std::string				name;
		std::vector<message>	error;
		std::vector<message>	warning;

		void	print_errors(bool last) {
			std::cout << "\033[1;97mFile: \033[0m" << std::setw(4 + name.size()) << name << std::endl;
			for (int i = 0; i < error.size(); i++) {
				std::cout << std::setw(20) << std::right << "\033[0;31mError: \033[0m"
					<< std::setw(10) << std::left << (error[i].line != "" ? "line " + error[i].line : "")
					<< std::setw(10) << (error[i].column != "" ? "col " + error[i].column : "")
					<< std::setw(6 + error[i].description.size()) << error[i].description
					<< std::endl;
			}
			if (warning.size() > 0 && error.size() > 0)
				std::cout << std::endl;
			for (int i = 0; i < warning.size(); i++) {
				std::cout << std::setw(22) << "\033[0;33mWarning: \033[0m"
					<< std::setw(18 + warning[i].description.size()) << warning[i].description
					<< std::endl;
			}
			if (!last)
				std::cout << std::endl;
		};
};
