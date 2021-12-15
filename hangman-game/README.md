# The Hangman Game using Sockets

This project is an implementation of a **Client Server** Hangman Game using **Sockets**.

## Requirements

These requirements must be installed on your machine to run the project.

- gcc (v11.1)
- make (v4.3)

## Commands to build and run the application

To make the build and run process easier, a makefile is provided to assist it. The commands below **must be executed in the project's root folder**.

### Build

We need to generate the executable files. So, to **build** the project, just run the following command:

```
make build
```

### Clean

To delete the executable files generated in the **build** process, run the following command:

```
make clean
```

### Run Server

To run the **server** application after the `make build`, run the following command:

```
make run_server
```

The server supports up to `5` simultaneous clients, defined by the `NUM_THREADS` variable in the `servidor.c` file.

### Run Client

To run the **client** application after the `make build`, run the following command:

```
make run_client
```

#### Execution

While the client is running, you can enter characters to guess which word is being presented in the game. Client closes when game is over.

### Help

Run the following command to display usage information.

```
make help
```
