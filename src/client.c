/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybolivar <ybolivar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 09:44:43 by airyago           #+#    #+#             */
/*   Updated: 2024/05/30 16:17:59 by ybolivar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int	g_control = 0;

/**
 * @brief Waits for a signal from the server that indicates it's ready to
 * receive more data. This function implements a timeout mechanism
 * that exits the program if the server does not respond within a certain
 * time frame.
 *
 * @details The function polls the global control variable `g_control`,
 * which is set by the signal handler `sig_response_handler` when a signal is
 * received. If `g_control` remains 0 for more than 5 seconds,
 * the function triggers a timeout error.
 */
static void	await_signal_confirmation(void)
{
	int	i;

	i = 0;
	while (g_control == 0)
	{
		if (i == 50)
			exit_on_error("Timeout");
		usleep(100);
		++i;
	}
	g_control = 0;
}

/**
 * @brief Sends a single character to the server by transmitting each
 * bit as a signal
 *
 * @param c The character to be sent to the server
 * @param server_pid The process ID of the server to which the
 * signals are sent
 *
 * @details This function iterates over each bit of the character `c`.
 * For each bit, it sends SIGUSR1 if the bit is 1, or SIGUSR2 if
 * the bit is 0. After sending each signal, it calls 
 * `await_signal_confirmation()` to ensure the server has processed 
 * the signal before proceeding.
 */
static void	send_char_as_sig(unsigned char c, pid_t server_pid)
{
	unsigned char	c_mask;

	c_mask = 0b10000000;
	while (c_mask)
	{
		if (c_mask & c)
		{
			if (kill(server_pid, SIGUSR1) == -1)
				exit_on_error("Could not send signal:\
check server availability or pid validity");
		}
		else
		{
			if (kill(server_pid, SIGUSR2) == -1)
				exit_on_error("Could not send signal:\
check server availability or pid validity");
		}
		await_signal_confirmation();
		c_mask >>= 1;
	}
}

/**
 * @brief Handles incoming signals from the server, setting a control flag
 *
 * @param signum The signal number received
 *
 * @details This handler sets `g_control` to 1 upon receiving any signal,
 * indicating that a signal was received. If the signal is SIGUSR2,
 * it also outputs a confirmation message to standard output.
 */
static void	sig_response_handler(int signum)
{
	g_control = 1;
	if (signum == SIGUSR2)
		write(1, "Confirmation received\n", 23);
}

/**
 * @brief Sends a string to the server character by character.
 *
 * @param pid The process ID of the server
 * @param str The string to be sent
 * @return Returns 1 on success
 *
 * @details This function iterates over each character in the
 * string `str` and uses `send_char_as_sig` to send
 * each character. After all characters are sent, it
 * sends a null character to signify the end of the message.
 */
static int	send_str_to_server(pid_t pid, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		send_char_as_sig(str[i], pid);
		++i;
	}
	send_char_as_sig(0, pid);
	return (1);
}

/**
 * @brief Main entry point of the client program. Sets up signal handling
 * and initiates message sending.
 *
 * @param argc The count of command-line arguments
 * @param argv The command-line arguments
 * @return Returns 1 on success, 0 on failure
 *
 * @details This function checks for correct usage, sets up signal handling
 * for SIGUSR1 and SIGUSR2, and sends a specified message to a server
 * identified by PID. If any setup fails or the message sending fails,
 * it exits with an error.
 */
int	main(int argc, char **argv)
{
	pid_t				pid;
	struct sigaction	client_sig;

	if (argc != 3 || !is_pid(argv[1]) || !argv[2])
		exit_on_error("Usage: ./client <pid> message to send.");
	pid = (pid_t)ft_atoi(argv[1]);
	sigemptyset(&client_sig.sa_mask);
	client_sig.sa_flags = 0;
	client_sig.sa_handler = sig_response_handler;
	if (sigaction(SIGUSR1, &client_sig, NULL) == -1)
	{
		exit_on_error("Failed to modify SIGUSR2 behavior. \
Please check system permissions and signal settings.\n");
	}
	if (sigaction(SIGUSR2, &client_sig, NULL) == -1)
	{
		exit_on_error("Failed to modify SIGUSR2 behavior. \
Please check system permissions and signal settings.\n");
	}
	if (!send_str_to_server(pid, argv[2]))
		return (0);
	return (1);
}
