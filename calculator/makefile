help:
	@ echo Usage: make [option]
	@ echo
	@ echo where the options available are:
	@ echo build
	@ echo clean
	@ echo help
	@ echo run_client
	@ echo run_server

CLASS = $(shell find . -name "*.class" -type f -delete)
JAVA = $(shell find . -name "*.java" -type f)

clean:
	@ $(CLASS)

build:
	@ javac $(JAVA)

run_server:
	@ java src.Servidor

run_client:
	@ java src.Cliente