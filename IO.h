#pragma once
#include "Head.h"
#include "StringUtils.h"

class IO {
public:

	static const int IO_ERR = -1;
	static const int BUFFER_SIZE = 1 << 18;
	
	bool eof;
	char curChar;

	IO() : rbCount(0), rbOffset(0), curChar(0), eof(false) {}

	~IO() {
		fwrite(writeBuffer, sizeof(writeBuffer[0]), wbOffset, stdout);
	}

	inline void shiftChar();
	inline void skipWS();
	inline void skipToEndl();

	inline int nextInt();
	inline uint nextUint();
	inline ll nextLong();
	inline ull nextUlong();
	inline char nextCharWS();
	inline char nextChar();
	inline void nextString(char * s);
	inline void nextLine(char * s);

	IO& operator >>(int& x);
	IO& operator >>(uint& x);
	IO& operator >>(ll& x);
	IO& operator >>(ull& x);
	IO& operator >>(char& x);
	IO& operator >>(char * s);

	void flush();
	void newline();

	void printInt(int x);
	void printUint(uint x);
	void printLong(ll x);
	void printUlong(ull x);
	void printChar(const char ch);
	void printString(const char * s);
	void printLine(const char * s);

	IO& operator <<(int x);
	IO& operator <<(uint x);
	IO& operator <<(ll x);
	IO& operator <<(ull x);
	IO& operator <<(const char x);
	IO& operator <<(const char * s);
	IO& operator <<(const string& s);
	IO& operator <<(ostream& (*fn)(ostream&));

	void assignFiles(const string& task) const;
	void assignFiles(const string& inputFile, const string& outputFile) const;
	void assignFilesInputTxt() const;

private:
	char readBuffer[BUFFER_SIZE], writeBuffer[BUFFER_SIZE];
	size_t rbCount, rbOffset, wbOffset;

};

extern bool useFastIO;
extern istream * pin;
extern ostream * pout;
extern IO io;

inline int nextInt() {
	if (useFastIO) {
		return io.nextInt();
	}
	int ret;
	*pin >> ret;
	return ret;
}

inline uint nextUint() {
	if (useFastIO) {
		return io.nextUint();
	}
	uint ret;
	*pin >> ret;
	return ret;
}

inline ll nextLong() {
	if (useFastIO) {
		return io.nextLong();
	}
	ll ret;
	*pin >> ret;
	return ret;
}

inline ull nextUlong() {
	if (useFastIO) {
		return io.nextUlong();
	}
	ull ret;
	*pin >> ret;
	return ret;
}

inline void skipWS() {
	if (useFastIO) {
		io.skipWS();
		return;
	}
	*pin >> noskipws;
	char ch;
	while (*pin >> ch) {
		if (ch != ' ' && ch != '\n') {
			pin->unget();
			break;
		}
	}
}

inline void skipToEndl() {
	if (useFastIO) {
		io.skipToEndl();
		return;
	}
	string s;
	getline(*pin, s);
}

inline char nextCharWS() {
	if (useFastIO) {
		return io.nextCharWS();
	}
	char ch;
	*pin >> noskipws >> ch >> skipws;
	return ch;
}

inline char nextChar() {
	if (useFastIO) {
		return io.nextChar();
	}
	char ch;
	*pin >> skipws >> ch;
	return ch;
}

inline void nextString(char * s) {
	if (useFastIO) {
		io.nextString(s);
		return;
	}
	*pin >> s;
}

inline void nextLine(char * s) {
	if (useFastIO) {
		io.nextLine(s);
		return;
	}
	string st;
	getline(*pin, st);
	strcpy(s, st.c_str());
}

inline void flush() {
	if (useFastIO) {
		io.flush();
		return;
	}
	*pout << flush;
}

inline void newline() {
	if (useFastIO) {
		io.newline();
		return;
	}
	*pout << endl;
}

inline void printInt(int x) {
	if (useFastIO) {
		io.printInt(x);
		return;
	}
	*pout << x;
}

inline void printUint(uint x) {
	if (useFastIO) {
		io.printUint(x);
		return;
	}
	*pout << x;
}

inline void printLong(ll x) {
	if (useFastIO) {
		io.printLong(x);
		return;
	}
	*pout << x;
}

inline void printUlong(ull x) {
	if (useFastIO) {
		io.printUlong(x);
		return;
	}
	*pout << x;
}

inline void printChar(const char x) {
	if (useFastIO) {
		io.printChar(x);
		return;
	}
	*pout << x;
}

inline void printString(const char * s) {
	if (useFastIO) {
		io.printString(s);
		return;
	}
	*pout << s;
}

inline void printLine(const char * s) {
	if (useFastIO) {
		io.printLine(s);
		return;
	}
	*pout << s << endl;
}
