package utils;

public enum Operadores {
	SUM('+'),
	SUB('-'),
	MUL('*'),
	DIV('/');
	char symbol;

	Operadores(char symbol) {
		this.symbol = symbol;
	}

	public static Operadores getByOperator(char symbol)	{
		for (Operadores operator : Operadores.values()) {
			if (operator.symbol == symbol) {
				return operator;
			}
		}
		return null;
	}
}
