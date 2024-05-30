/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybolivar <ybolivar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 14:33:59 by aherbin           #+#    #+#             */
/*   Updated: 2024/05/30 16:20:00 by ybolivar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

// Set as static for exposure control
static void	handle_sig_data(int signum, siginfo_t *info, void *ucontent);

/**
 * @brief Displays the server's process id
 *
 * @param pid Process id
 */
static void	display_server_pid(pid_t pid)
{
	write(1, "PID: ", 5);
	ft_putnbr_fd(pid, 1);
	write(1, "\n", 1);
}

/**
 * @brief Outputs the received character and communicates
 * back to the client confirming receipt.
 * Depending on the value of i sends SIGUSR 1 or 2
 * back to the client using kill.
 * Calls handle_sig_data(0, NULL, NULL) to reset the signal handler's
 * state for receiving the next character.
 *
 * @param c Character to be printed
 * @param i Signal flag
 * @param pid Client's process id
 */
static void	process_respond_client(char c, int i, pid_t pid)
{
	write(1, &c, 1);
	if (i == 0)
	{
		if (kill(pid, SIGUSR2) == -1)
			exit_on_error("The signal has not been sent\n");
	}
	if (i == 1)
	{
		if (kill(pid, SIGUSR1) == -1)
			exit_on_error("The signal has not been sent\n");
	}
	handle_sig_data(0, NULL, NULL);
}

/**
 * @brief Receives bits of data from signals, constructs characters,
 * and decides how to handle completed characters.
 * Static variables i and bit construct characters
 * from bits sent by the client.
 * The function increments i or modifies bit based on whether SIGUSR1 or SIGUSR2
 * is received, interpreting these as binary 0 or 1, respectively.
 *
 * @param signum The signal number
 * @param info Provides context about the signal (such as the sender's PID)
 * @param ucontent sa_sigaction user-defined data. Void but required
 */
static void	handle_sig_data(int signum, siginfo_t *info, void *ucontent)
{
	static int	i;
	static char	bit;

	(void)ucontent;
	if (signum == 0)
	{
		i = 0;
		bit = 0;
		return ;
	}
	if (signum == SIGUSR2)
		++i;
	if (signum == SIGUSR1)
	{
		bit += 1 << (7 - i);
		++i;
	}
	if (i == 8)
	{
		if (bit == 0)
			return (process_respond_client('\n', 0, info->si_pid));
		return (process_respond_client(bit, 1, info->si_pid));
	}
	kill(info->si_pid, SIGUSR1);
}

/**
 * @brief Sets up the signal handlers for SIGUSR1 and SIGUSR2.
 * Initializes the sigaction structure to define signals
 * SIGUSR1 and SIGUSR2 handler function.
 * Sets up the signal mask (sa_mask) to block these two signals
 * during the execution of the handler to prevent nested signal handling.
 * handle_sig_data is assigned as the handler for both signals.
 *
 * SA_SIGINFO is a flag that changes the behavior of the signal handling
 * mechanism to pass three arguments to the signal handler instead of one,
 * providing access to a `siginfo_t` structure.
 */
static void	setup_sig_handlers(void)
{
	struct sigaction	server_sig;

	sigemptyset(&server_sig.sa_mask);
	sigaddset(&server_sig.sa_mask, SIGUSR1);
	sigaddset(&server_sig.sa_mask, SIGUSR2);
	server_sig.sa_sigaction = &handle_sig_data;
	server_sig.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &server_sig, NULL) == -1)
		exit_on_error("Failed to modify SIGUSR1 behavior. \
Please check system permissions and signal settings.\n");
	if (sigaction(SIGUSR2, &server_sig, NULL) == -1)
		exit_on_error("Failed to modify SIGUSR2 behavior. \
Please check system permissions and signal settings.\n");
}

int	main(void)
{
	setup_sig_handlers();
	display_server_pid((int) getpid());
	handle_sig_data(0, NULL, NULL);
	while (1)
		pause();
	return (0);
}
