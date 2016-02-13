#include "IO.h"

inline void IO::shiftChar() {
	if (rbOffset == rbCount) {
		rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
		if (rbCount <= 0) {
			eof = true;
			return;
		}
		rbOffset = 0;
	}
	curChar = readBuffer[rbOffset++];
}

inline void IO::skipWS() {
	while (!eof && (curChar == ' ' || curChar == '\n')) {
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				return;
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
}

inline void IO::skipToEndl() {
	while (!eof && curChar != '\n') {
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				return;
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	// shiftChar begin
	if (rbOffset == rbCount) {
		rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
		if (rbCount <= 0) {
			eof = true;
			return;
		}
		rbOffset = 0;
	}
	curChar = readBuffer[rbOffset++];
	// shiftChar end
}

inline int IO::nextInt() {
	while (!eof && !isDigit(curChar) && curChar != '-') {
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				return IO_ERR;
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	bool neg = false;
	if (curChar == '-') {
		neg = true;
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				return IO_ERR;
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	int result = 0;
	while (!eof && isDigit(curChar)) {
		result = (result << 3) + (result << 1) + curChar - '0';
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				return (neg ? -result : result);
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	return (neg ? -result : result);
}

inline uint IO::nextUint() {
	while (!eof && !isDigit(curChar)) {
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				return IO_ERR;
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	uint result = 0;
	while (!eof && isDigit(curChar)) {
		result = (result << 3) + (result << 1) + curChar - '0';
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				return result;
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	return result;
}

inline ll IO::nextLong()  {
	while (!eof && !isDigit(curChar) && curChar != '-') {
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				return IO_ERR;
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	bool neg = false;
	if (curChar == '-') {
		neg = true;
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				return IO_ERR;
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	ll result = 0;
	while (!eof && isDigit(curChar)) {
		result = (result << 3) + (result << 1) + curChar - '0';
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				return (neg ? -result : result);
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	return (neg ? -result : result);
}

inline ull IO::nextUlong() {
	while (!eof && !isDigit(curChar)) {
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				return IO_ERR;
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	ull result = 0;
	while (!eof && isDigit(curChar)) {
		result = (result << 3) + (result << 1) + curChar - '0';
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				return result;
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	return result;
}

inline char IO::nextCharWS() {
	if (!curChar) {
		rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
		if (rbCount <= 0) {
			eof = true;
			return IO_ERR;
		}
		rbOffset = 0;
		curChar = readBuffer[rbOffset++];
	}
	char ret = curChar;
	// shiftChar begin
	if (rbOffset == rbCount) {
		rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
		if (rbCount <= 0) {
			eof = true;
			return ret;
		}
		rbOffset = 0;
	}
	curChar = readBuffer[rbOffset++];
	// shiftChar end
	return ret;
}

inline char IO::nextChar() {
	if (!curChar) {
		rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
		if (rbCount <= 0) {
			eof = true;
			return IO_ERR;
		}
		rbOffset = 0;
		curChar = readBuffer[rbOffset++];
	}
	while (!eof && (curChar == ' ' || curChar == '\n')) {
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				return IO_ERR;
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	char ret = curChar;
	// shiftChar begin
	if (rbOffset == rbCount) {
		rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
		if (rbCount <= 0) {
			eof = true;
			return ret;
		}
		rbOffset = 0;
	}
	curChar = readBuffer[rbOffset++];
	// shiftChar end
	return ret;
}

inline void IO::nextString(char * s) {
	if (!curChar) {
		rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
		if (rbCount <= 0) {
			eof = true;
			return;
		}
		rbOffset = 0;
		curChar = readBuffer[rbOffset++];
	}
	while (!eof && (curChar == ' ' || curChar == '\n')) {
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				return;
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	int ind = 0;
	while (!eof && curChar != ' ' && curChar != '\n') {
		s[ind++] = curChar;
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				s[ind] = 0;
				return;
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	s[ind] = 0;
}

inline void IO::nextLine(char * s) {
	if (!curChar) {
		rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
		if (rbCount <= 0) {
			eof = true;
			return;
		}
		rbOffset = 0;
		curChar = readBuffer[rbOffset++];
	}
	int ind = 0;
	while (!eof && curChar != '\n') {
		s[ind++] = curChar;
		// shiftChar begin
		if (rbOffset == rbCount) {
			rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
			if (rbCount <= 0) {
				eof = true;
				s[ind] = 0;
				return;
			}
			rbOffset = 0;
		}
		curChar = readBuffer[rbOffset++];
		// shiftChar end
	}
	s[ind] = 0;
	// shiftChar begin
	if (rbOffset == rbCount) {
		rbCount = fread(readBuffer, sizeof(readBuffer[0]), sizeof(readBuffer), stdin);
		if (rbCount <= 0) {
			eof = true;
			return;
		}
		rbOffset = 0;
	}
	curChar = readBuffer[rbOffset++];
	// shiftChar end
}

IO& IO::operator >>(int& x) {
	x = ::nextInt();
	return *this;
}

IO& IO::operator >>(uint& x) {
	x = ::nextUint();
	return *this;
}

IO& IO::operator >>(ll& x) {
	x = ::nextLong();
	return *this;
}

IO& IO::operator >>(ull& x) {
	x = ::nextUlong();
	return *this;
}

IO& IO::operator >>(char& x) {
	x = ::nextChar();
	return *this;
}

IO& IO::operator >>(char * s) {
	::nextString(s);
	return *this;
}

void IO::assignFiles(const string& task) const {
	if (useFastIO) {
		freopen((task + ".in").c_str(), "r", stdin);
		freopen((task + ".out").c_str(), "w", stdout);
	}
}

void IO::assignFiles(const string& inputFile, const string& outputFile) const {
	if (useFastIO) {
		freopen(inputFile.c_str(), "r", stdin);
		freopen(outputFile.c_str(), "w", stdout);
	}
}

void IO::assignFilesInputTxt() const {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
}

void IO::flush() {
	fwrite(writeBuffer, sizeof(writeBuffer[0]), wbOffset, stdout);
	wbOffset = 0;
}

void IO::newline() {
	if (wbOffset == BUFFER_SIZE) {
		// flush begin
		fwrite(writeBuffer, sizeof(writeBuffer[0]), wbOffset, stdout);
		wbOffset = 0;
		// flush end
	}
	writeBuffer[wbOffset++] = '\n';
}

void IO::printInt(int x) {
	if (wbOffset + 12 > BUFFER_SIZE) {
		// flush begin
		fwrite(writeBuffer, sizeof(writeBuffer[0]), wbOffset, stdout);
		wbOffset = 0;
		// flush end
	}
	if (x < 0) {
		writeBuffer[wbOffset++] = '-';
		x = -x;
	}
	else
	if (!x) {
		writeBuffer[wbOffset++] = '0';
		return;
	}
	int st = wbOffset;
	while (x) {
		writeBuffer[wbOffset++] = x % 10 + '0';
		x /= 10;
	}
	for (int l = st, r = wbOffset - 1; l < r; ++l, --r) {
		char t = writeBuffer[l];
		writeBuffer[l] = writeBuffer[r];
		writeBuffer[r] = t;
	}
}

void IO::printUint(uint x) {
	if (wbOffset + 12 > BUFFER_SIZE) {
		// flush begin
		fwrite(writeBuffer, sizeof(writeBuffer[0]), wbOffset, stdout);
		wbOffset = 0;
		// flush end
	}
	if (!x) {
		writeBuffer[wbOffset++] = '0';
		return;
	}
	int st = wbOffset;
	while (x) {
		writeBuffer[wbOffset++] = x % 10 + '0';
		x /= 10;
	}
	for (int l = st, r = wbOffset - 1; l < r; ++l, --r) {
		char t = writeBuffer[l];
		writeBuffer[l] = writeBuffer[r];
		writeBuffer[r] = t;
	}
}

void IO::printLong(ll x) {
	if (wbOffset + 22 > BUFFER_SIZE) {
		// flush begin
		fwrite(writeBuffer, sizeof(writeBuffer[0]), wbOffset, stdout);
		wbOffset = 0;
		// flush end
	}
	if (x < 0) {
		writeBuffer[wbOffset++] = '-';
		x = -x;
	}
	else
	if (!x) {
		writeBuffer[wbOffset++] = '0';
		return;
	}
	int st = wbOffset;
	while (x) {
		writeBuffer[wbOffset++] = x % 10 + '0';
		x /= 10;
	}
	for (int l = st, r = wbOffset - 1; l < r; ++l, --r) {
		char t = writeBuffer[l];
		writeBuffer[l] = writeBuffer[r];
		writeBuffer[r] = t;
	}
}

void IO::printUlong(ull x) {
	if (wbOffset + 22 > BUFFER_SIZE) {
		// flush begin
		fwrite(writeBuffer, sizeof(writeBuffer[0]), wbOffset, stdout);
		wbOffset = 0;
		// flush end
	}
	if (!x) {
		writeBuffer[wbOffset++] = '0';
		return;
	}
	int st = wbOffset;
	while (x) {
		writeBuffer[wbOffset++] = x % 10 + '0';
		x /= 10;
	}
	for (int l = st, r = wbOffset - 1; l < r; ++l, --r) {
		char t = writeBuffer[l];
		writeBuffer[l] = writeBuffer[r];
		writeBuffer[r] = t;
	}
}

void IO::printChar(const char x) {
	if (wbOffset == BUFFER_SIZE) {
		// flush begin
		fwrite(writeBuffer, sizeof(writeBuffer[0]), wbOffset, stdout);
		wbOffset = 0;
		// flush end
	}
	writeBuffer[wbOffset++] = x;
}

void IO::printString(const char * s) {
	for (int i = 0; s[i]; ++i) {
		if (wbOffset == BUFFER_SIZE) {
			// flush begin
			fwrite(writeBuffer, sizeof(writeBuffer[0]), wbOffset, stdout);
			wbOffset = 0;
			// flush end
		}
		writeBuffer[wbOffset++] = s[i];
	}
}

void IO::printLine(const char * s) {
	for (int i = 0; s[i]; ++i) {
		if (wbOffset == BUFFER_SIZE) {
			// flush begin
			fwrite(writeBuffer, sizeof(writeBuffer[0]), wbOffset, stdout);
			wbOffset = 0;
			// flush end
		}
		writeBuffer[wbOffset++] = s[i];
	}
	if (wbOffset == BUFFER_SIZE) {
		// flush begin
		fwrite(writeBuffer, sizeof(writeBuffer[0]), wbOffset, stdout);
		wbOffset = 0;
		// flush end
	}
	writeBuffer[wbOffset++] = '\n';
}

IO& IO::operator <<(int x) {
	::printInt(x);
	return *this;
}

IO& IO::operator <<(uint x) {
	::printUint(x);
	return *this;
}

IO& IO::operator <<(ll x) {
	::printLong(x);
	return *this;
}

IO& IO::operator <<(ull x) {
	::printUlong(x);
	return *this;
}

IO& IO::operator <<(const char x) {
	::printChar(x);
	return *this;
}

IO& IO::operator <<(const char * s) {
	::printString(s);
	return *this;
}

IO& IO::operator <<(const string& s) {
	::printString(s.c_str());
	return *this;
}

IO& IO::operator <<(ostream& (*fn)(ostream&)) {
	::newline();
	return *this;
}
