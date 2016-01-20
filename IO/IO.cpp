#include "IO/IO.h"

extern bool fastIO;

inline void IO::SkipWS() {
	while (!eof_ && (current_char_ == ' ' || current_char_ == '\n')) {
		ShiftChar();
	}
}

inline void IO::SkipToEndl() {
	while (!eof_ && current_char_ != '\n') {
		ShiftChar();
	}
	ShiftChar();
}

template<typename T>
T IO::ReadNumber() {
	bool neg = false;
	while (!eof_ && !isDigit(current_char_) && (std::is_unsigned<T>() || current_char_ != '-')) {
		ShiftChar();
	}
	if (std::is_signed<T>() && current_char_ == '-') {
		neg = true;
		ShiftChar();
	}
	T result = 0;
	while (!eof_ && isDigit(current_char_)) {
		result = (result << 3) + (result << 1) + current_char_ - '0';
		ShiftChar();
	}
	return (neg ? result * -1 : result);
}

template<typename T>
void IO::PrintNumber(const T& value) {
	T x = value;
	if (write_bytes_offset_ + 32 > kBufferSize) {
		Flush();
	}
	if (x < 0) {
		write_buffer_[write_bytes_offset_++] = '-';
		x *= -1;
	}
	else
	if (x == 0) {
		write_buffer_[write_bytes_offset_++] = '0';
		return;
	}
	std::size_t start_index = write_bytes_offset_;
	while (x != 0) {
		write_buffer_[write_bytes_offset_++] = x % 10 + '0';
		x /= 10;
	}
	std::reverse(write_buffer_.begin() + start_index, write_buffer_.begin() + write_bytes_offset_);
}

inline void IO::NextLine(char* s) {
	if (current_char_ == '\0') {
		ShiftChar();
	}
	std::size_t ind = 0;
	while (!eof_ && current_char_ != '\n') {
		s[ind++] = current_char_;
		ShiftChar();
	}
	s[ind] = 0;
	ShiftChar();
}

inline void IO::UpdateBuffer() {
	if (read_bytes_offset_ == read_bytes_count_) {
		read_bytes_count_ = fread(&read_buffer_[0], sizeof(read_buffer_[0]), read_buffer_.size(), stdin);
		if (read_bytes_count_ == 0) {
			eof_ = true;
			return;
		}
		read_bytes_offset_ = 0;
	}
}

inline void IO::ShiftChar() {
	UpdateBuffer();
	current_char_ = read_buffer_[read_bytes_offset_++];
}

inline int IO::NextInt() {
	return ReadNumber<int>();
}

inline unsigned int IO::NextUint() {
	return ReadNumber<unsigned int>();
}

inline long long IO::NextLong()  {
	return ReadNumber<long long>();
}

inline unsigned long long IO::NextUlong() {
	return ReadNumber<unsigned long long>();
}

inline char IO::NextCharWS() {
	if (!current_char_) {
		ShiftChar();
	}
	char ret = current_char_;
	ShiftChar();
	return ret;
}

inline char IO::NextChar() {
	if (!current_char_) {
		ShiftChar();
	}
	SkipWS();
	char ret = current_char_;
	ShiftChar();
	return ret;
}

inline void IO::NextString(char* s) {
	if (!current_char_) {
		ShiftChar();
	}
	SkipWS();
	int ind = 0;
	while (!eof_ && current_char_ != ' ' && current_char_ != '\n') {
		s[ind++] = current_char_;
		ShiftChar();
	}
	s[ind] = 0;
}

IO& IO::operator >>(int& x) {
	x = ::NextInt();
	return *this;
}

IO& IO::operator >>(unsigned int& x) {
	x = ::NextUint();
	return *this;
}

IO& IO::operator >>(long long& x) {
	x = ::NextLong();
	return *this;
}

IO& IO::operator >>(unsigned long long& x) {
	x = ::NextUlong();
	return *this;
}

IO& IO::operator >>(char& x) {
	x = ::NextChar();
	return *this;
}

IO& IO::operator >>(char * s) {
	::NextString(s);
	return *this;
}

void IO::assignFiles(const std::string& task) const {
	freopen((task + ".in").c_str(), "r", stdin);
	freopen((task + ".out").c_str(), "w", stdout);
}

void IO::assignFiles(const std::string& inputFile, const std::string& outputFile) const {
	freopen(inputFile.c_str(), "r", stdin);
	freopen(outputFile.c_str(), "w", stdout);
}

void IO::assignFilesInputTxt() const {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
}

inline void IO::Flush() {
	fwrite(&write_buffer_[0], sizeof(write_buffer_[0]), write_bytes_offset_, stdout);
	write_bytes_offset_ = 0;
}

inline void IO::NewLine() {
	if (write_bytes_offset_ == kBufferSize) {
		Flush();
	}
	write_buffer_[write_bytes_offset_++] = '\n';
}

inline void IO::PrintInt(const int& x) {
	PrintNumber(x);
}

inline void IO::PrintUint(const unsigned int& x) {
	PrintNumber(x);
}

inline void IO::PrintLong(const long long& x) {
	PrintNumber(x);
}

inline void IO::PrintUlong(const unsigned long long& x) {
	PrintNumber(x);
}

inline void IO::PrintChar(const char& x) {
	if (write_bytes_offset_ == kBufferSize) {
		Flush();
	}
	write_buffer_[write_bytes_offset_++] = x;
}

inline void IO::PrintString(const char* s) {
	for (int i = 0; s[i]; ++i) {
		if (write_bytes_offset_ == kBufferSize) {
			Flush();
		}
		write_buffer_[write_bytes_offset_++] = s[i];
	}
}

inline void IO::PrintLine(const char* s) {
	for (int i = 0; s[i]; ++i) {
		if (write_bytes_offset_ == kBufferSize) {
			Flush();
		}
		write_buffer_[write_bytes_offset_++] = s[i];
	}
	if (write_bytes_offset_ == kBufferSize) {
		Flush();
	}
	write_buffer_[write_bytes_offset_++] = '\n';
}

IO& IO::operator <<(const int& x) {
	::PrintInt(x);
	return *this;
}

IO& IO::operator <<(const unsigned int& x) {
	::PrintUint(x);
	return *this;
}

IO& IO::operator <<(const long long& x) {
	::PrintLong(x);
	return *this;
}

IO& IO::operator <<(const unsigned long long& x) {
	::PrintUlong(x);
	return *this;
}

IO& IO::operator <<(const char& x) {
	::PrintChar(x);
	return *this;
}

IO& IO::operator <<(const char* s) {
	::PrintString(s);
	return *this;
}

IO& IO::operator <<(const std::string& s) {
	::PrintString(s.c_str());
	return *this;
}

IO& IO::operator <<(ostream& (*fn)(ostream&)) {
	::NewLine();
	return *this;
}

inline int NextInt() {
	if (useFastIO) {
		return io.NextInt();
	}
	int ret;
	*pin >> ret;
	return ret;
}

inline unsigned int NextUint() {
	if (useFastIO) {
		return io.NextUint();
	}
	unsigned int ret;
	*pin >> ret;
	return ret;
}

inline long long NextLong() {
	if (useFastIO) {
		return io.NextLong();
	}
	long long ret;
	*pin >> ret;
	return ret;
}

inline unsigned long long NextUlong() {
	if (useFastIO) {
		return io.NextUlong();
	}
	unsigned long long ret;
	*pin >> ret;
	return ret;
}

inline void SkipWS() {
	if (useFastIO) {
		io.SkipWS();
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

inline void SkipToEndl() {
	if (useFastIO) {
		io.SkipToEndl();
		return;
	}
	std::string s;
	getline(*pin, s);
}

inline char NextCharWS() {
	if (useFastIO) {
		return io.NextCharWS();
	}
	char ch;
	*pin >> noskipws >> ch >> skipws;
	return ch;
}

inline char NextChar() {
	if (useFastIO) {
		return io.NextChar();
	}
	char ch;
	*pin >> skipws >> ch;
	return ch;
}

inline void NextString(char* s) {
	if (useFastIO) {
		io.NextString(s);
		return;
	}
	*pin >> s;
}

inline void NextLine(char* s) {
	if (useFastIO) {
		io.NextLine(s);
		return;
	}
	std::string st;
	getline(*pin, st);
	strcpy(s, st.c_str());
}

inline void Flush() {
	if (useFastIO) {
		io.Flush();
		return;
	}
	*pout << flush;
}

inline void NewLine() {
	if (useFastIO) {
		io.NewLine();
		return;
	}
	*pout << endl;
}

inline void PrintInt(const int& x) {
	if (useFastIO) {
		io.PrintInt(x);
		return;
	}
	*pout << x;
}

inline void PrintUint(const unsigned int& x) {
	if (useFastIO) {
		io.PrintUint(x);
		return;
	}
	*pout << x;
}

inline void PrintLong(const long long& x) {
	if (useFastIO) {
		io.PrintLong(x);
		return;
	}
	*pout << x;
}

inline void PrintUlong(const unsigned long long& x) {
	if (useFastIO) {
		io.PrintUlong(x);
		return;
	}
	*pout << x;
}

inline void PrintChar(const char& x) {
	if (useFastIO) {
		io.PrintChar(x);
		return;
	}
	*pout << x;
}

inline void PrintString(const char* s) {
	if (useFastIO) {
		io.PrintString(s);
		return;
	}
	*pout << s;
}

inline void PrintLine(const char* s) {
	if (useFastIO) {
		io.PrintLine(s);
		return;
	}
	*pout << s << endl;
}
