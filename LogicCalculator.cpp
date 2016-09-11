#include <iostream>
#include <stdexcept>
using namespace std;

struct Token {
	char kind;
	bool value;
	Token(char ch) : kind(ch), value(true) {}
	Token(char ch, bool val) : kind(ch), value(val) {}
};

class Token_stream {
	bool full;
	Token buffer;

public:
	Token_stream() : full(false), buffer(0) {}
	void putback(Token t) {
		if (full) throw runtime_error("putback into a full buffer");
		buffer = t;
		full = true;
	}
	Token get() {
		if (full) { full = false; return buffer; }
		char ch;
		cin >> ch;
		switch (ch) {
			//possible cases
		case ('Q') : case ('q') :

		case '(': case ')': case '&': case '|':

		case '^': case '!': case ';': case '=':

			return Token(ch);

		case 'T': case 'F': case 't': case 'f': {
			cin.putback(ch); 
			char val;
			cin >> val; 

			if (val == 'T' || val == 't') {
				return Token('8', true);
			}
			if (val == 'F' || val == 'f') {
				return Token('8', false);
			}
		}
		//unexpexted input
		default: throw runtime_error("Bad token");
		}
	}
};

Token_stream ts; //declare object of class Token_stream

bool expression();  // declaration to deal with indirect recursion

bool primary() {
	Token t = ts.get();
	switch (t.kind) {
	//Deal with paranthesis
	case '(': {
		bool d = expression();
		t = ts.get();
		if (t.kind != ')') throw runtime_error("')' expected");
		return d;
	}
	//Deal with NOT case
	case '!': {
		bool exp = expression();
		//exp = !exp;
		return !exp;
	}

	case '8': return t.value;

	default: {
		throw runtime_error("primary expected");
	}
	}
}

bool factor() {
	bool val = primary();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		//Deal with XOR gate
		case '^': val = val ^ factor(); break;

		default: ts.putback(t); return val;
		}
	}
}

bool term() {
	bool val = factor();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		//Deal with AND gate
		case '&': val = val & factor(); break;

		default: ts.putback(t); return val;
		}
	}
}

bool expression() {
	bool val = term();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		//Deal with OR gate
		case '|': val = val | term(); break;

		default: ts.putback(t); return val;
		}
	}
}

void Calculate(bool& userContinue) {
	try {
		bool val = true;
		while (userContinue == true) {
			Token t = ts.get();
			if (t.kind == ';' || t.kind == '=') {
				if (val == true) {
					cout << "T" << endl;
				}
				else {
					cout << "F" << endl;
				}
				return;
				}
			if (t.kind == 'q' || t.kind == 'Q') {
				userContinue = false;
				break;
			}
			else
				ts.putback(t);
				val = expression();
		}
	}
	catch (runtime_error& e) {
		cerr << e.what() << endl;
	}
	catch (...) {
		cerr << "exception\n";
	}
}

int main() {
	bool userContinue = true;
	while (userContinue) {
		Calculate(userContinue);
	}
	return 0;
}