/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybolivar <ybolivar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 14:33:49 by aherbin           #+#    #+#             */
/*   Updated: 2024/05/30 15:49:06 by ybolivar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <unistd.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/types.h>

// Function prototypes
int		ft_atoi(char *str);					// Convert string to integer
int		ft_isdigit(int c);					// Check if char is decimal digit
void	ft_putnbr_fd(pid_t nb, int fd);	// Print integer nb to fd
void	exit_on_error(char *error_code);	// Print error_code and exit
int		is_pid(char *spid);					// Check spid is valid process ID

#endif
