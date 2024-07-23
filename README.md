# Minitalk

Minitalk is a simple communication program which implements a server and a client to send and receive messages using UNIX signals. The project utilizes `SIGUSR1` and `SIGUSR2` signals to transmit messages bit by bit across processes.

## Features

- **Simple and Lightweight Message Exchange**: Messages are sent one character at a time through signals.
- **Server-Client Architecture**: Separate executable files for server and client.
- **Error Handling**: Includes basic error handling for timeouts and signal sending errors.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

To run this project, you need to have a GCC compiler and Make installed on your system. This project is developed on a Unix-based system and may not function as intended on non-Unix systems.

### Installation

1. **Clone the Repository**

    ```
    git clone https://github.com/yourusername/minitalk.git
    cd minitalk
    ```

2. **Compile the Project**

    Using the Makefile included in the project directory, you can easily compile the server and client:

    ```
    make
    ```

    This will create two executables: `server` and `client`.

### Running the Server

To run the server, use the following command:

```
./server
```

Upon starting, the server will display its PID. Keep note of this PID as it will be used to send messages from the client.

### Running the Client

To send a message to the server, run the client with the server's PID and the message as arguments:

```
./client <Server_PID> "Hello, World!"
```

Replace `<Server_PID>` with the actual PID displayed by the server.

## Usage Example

Start the server:

```
$ ./server
PID: 12345
```

In another terminal, send a message from the client:

```
$ ./client 12345 "Hello, Minitalk!"
```

The server will print the received message:

```
Hello, Minitalk!
```

## Contributing

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

Distributed under the MIT License. See `LICENSE` for more information.