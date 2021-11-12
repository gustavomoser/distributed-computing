package servidor;

import java.io.IOException;
import java.net.ServerSocket;

public class Servidor {

	public static final int SERV_PORT = 6789;
	public static final int SERV_THREADS = 4;

	public Servidor(int porta, int numThreads) {
		ServerSocket server;

		try {
			server = new ServerSocket(porta);
		} catch(IOException e) {
			throw new RuntimeException("Não foi possível instanciar o ServerSocket ", e);
		}

		for (int i = 0; i < numThreads; i++) {
			new ClienteServerHandler(server, i).start();
		}
	}

	public static void main(String[] args) {
		new Servidor(SERV_PORT, SERV_THREADS);
	}
}
