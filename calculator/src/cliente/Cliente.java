package cliente;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.Scanner;

import utils.Operadores;

public class Cliente {

	public static final int CLI_PORT = 6789;
	public static final String CLI_HOST = "localhost";
	public static final String SAIR = "sair";

	public Cliente(String hostname, int porta) {
		try {
			Socket socket = new Socket(hostname, porta);

			DataInputStream input = new DataInputStream(socket.getInputStream());
			DataOutputStream output = new DataOutputStream(socket.getOutputStream());

			Scanner scanner = new Scanner(System.in);

			while (true) {
				String operation = scanner.nextLine();

				if (operation.equals(SAIR)) {
					break;
				} else {
					Operadores operator = this.getOperation(operation);

					if (operator == null) {
						System.out.println("Invalid operator!");
					} else {
						String[] numbers;
						switch (operator) {
						case SUM:
							numbers = operation.split("\\+");
							output.writeUTF("+#" + numbers[0] + "#" + numbers[1]);
							break;
						case SUB:
							numbers = operation.split("-");
							output.writeUTF("-#" + numbers[0] + "#" + numbers[1]);
							break;
						case MUL:
							numbers = operation.split("\\*");
							output.writeUTF("*#" + numbers[0] + "#" + numbers[1]);
							break;
						case DIV:
							numbers = operation.split("/");
							output.writeUTF("/#" + numbers[0] + "#" + numbers[1]);
							break;
						}
						int response = input.readInt();
						System.out.println(response);
					}
				}
			}
		} catch (IOException e) {
			System.out.println(this.getClass().getName() + ": Erro de IO no socket - " + e);
		}
	}

	private Operadores getOperation(String input) {
		if (input.matches("(\\d+\\+\\d+)")) {
			return Operadores.SUM;
		} else if (input.matches("(\\d+-\\d+)")) {
			return Operadores.SUB;
		} else if (input.matches("(\\d+\\*\\d+)")) {
			return Operadores.MUL;
		} else if (input.matches("(\\d+/\\d+)")) {
			return Operadores.DIV;
		}
		return null;
	}

	public static void main(String[] args) {
		new Cliente(CLI_HOST, CLI_PORT);
	}
}
