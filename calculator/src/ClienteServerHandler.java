package src;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class ClienteServerHandler extends Thread {

	static final String SAIR = "sair";
	final ServerSocket server;
	Socket client;
	int threadIndex;

	public ClienteServerHandler(ServerSocket ss, int threadIndex) {
		this.server = ss;
		this.threadIndex = threadIndex;
		this.setName("thread_" + threadIndex);
	}

	@Override
	public void run() {
		while (true) {
			try {
				System.out.println(this.getName() + " está esperado...");

				synchronized(this.server) {
					this.client = this.server.accept();
				}

				System.out.println(this.getName() + " iniciou | IP: " + this.client.getInetAddress());

				DataInputStream input = new DataInputStream(this.client.getInputStream());
				DataOutputStream output = new DataOutputStream(this.client.getOutputStream());

				while (true) {
					String operation = input.readUTF();
					
					if (operation.equals(SAIR)) {
						break;
					} else {
						String[] parts = operation.split("#");
						char symbol = parts[0].charAt(0);
						Operadores op = Operadores.getByOperator(symbol);

						if (op == null) {
							System.out.println("Operador inválido!");
						} else {
							int number1 = Integer.parseInt(parts[1]);
							int number2 = Integer.parseInt(parts[2]);

							switch (op) {
							case SUM:
								output.writeInt(number1 + number2);
								break;
							case SUB:
								output.writeInt(number1 - number2);
								break;
							case MUL:
								output.writeInt(number1 * number2);
								break;
							case DIV:
								output.writeInt(number1 / number2);
								break;
							}
						}
					}
				}
			} catch (EOFException e) {
				try {
					this.client.close();
					System.out.println("Operação na " + this.getName() + " foi finalizada!");
				} catch (IOException ioe) {
					System.out.println(this.getName() + ": Erro ao fechar conexão com socket - " + ioe);
				}
			} catch (IOException e) {
				System.out.println(this.getName() + ": Erro de IO no socket - " + e);
			}
		}
	}
}
