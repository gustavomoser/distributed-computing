# Calculator using Socket

This project is an implementation of a **Client Server** Calculator using **Sockets**.

## Requirements

These requirements must be installed on your machine to run the project.

- Java - JDK (v8)
- Make (v4.3)

## Commands to build and run the application

To make the build and run process easier, a makefile is provided to assist it. The commands below **must be executed in the project's root folder**.

### Build

As Java project, we need the classes compiled before run the application. So, to **build** the project, just run the following command:

```
make build
```

### Clean

To delete the `.class` files generated in the **build** process, run the following command:

```
make clean
```

### Run Server

To run the **server** application after the `make build`, run the following command:

```
make run_server
```

The server supports up to `4` simultaneous clients, defined by the `SERV_THREADS` variable in the `Servidor` class.

### Run Client

To run the **client** application after the `make build`, run the following command:

```
make run_client
```

#### Execution

While the client is running, you can do simple math operations, which are: `+ - * /`, between two operands. To close the client, type `sair`.

### Help

Run the following command to display usage information.

```
make help
```
