help:
	@ echo Usage: make [option]
	@ echo
	@ echo where the options available are:
	@ echo build
	@ echo clean
	@ echo help
	@ echo run_client
	@ echo run_server

clean:
	@ rm servidor cliente

build:
	@ gcc cliente.c -o cliente
	@ gcc servidor.c -o servidor -lpthread

run_server:
	@ ./servidor

run_client:
	@ ./cliente