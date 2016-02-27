#include "IO/IO.h"

extern bool fastIO;

inline void IO::skipws() {
	while (!eof_ && (current_char_ == ' ' || current_char_ == '\n')) {
		shift_char();
	}
}

inline void IO::skip_to_endl() {
	while (!eof_ && current_char_ != '\n') {
		shift_char();
	}
	shift_char();
}

template<typename T>
T IO::read_number() {
	bool is_negative = false;
	while (!eof_ && !is_digit(current_char_) && (std::is_unsigned<T>() || current_char_ != '-')) {
		shift_char();
	}
	if (std::is_signed<T>() && current_char_ == '-') {
		is_negative = true;
		shift_char();
	}
	T result = 0;
	while (!eof_ && is_digit(current_char_)) {
		result = (result << 3) + (result << 1) + current_char_ - '0';
		shift_char();
	}
	return (is_negative ? result * -1 : result);
}

template<typename T>
void IO::print_number(const T& value) {
	T current_value = value;
	if (write_buffer_offset_ + 32 > kBufferSize) {
		flush();
	}
	if (current_value < 0) {
		write_buffer_[write_buffer_offset_++] = '-';
		current_value = abs(current_value);
	} else
	if (current_value == 0) {
		write_buffer_[write_buffer_offset_++] = '0';
		return;
	}
	std::size_t start_index = write_buffer_offset_;
	while (current_value != 0) {
		write_buffer_[write_buffer_offset_++] = current_value % 10 + '0';
		current_value /= 10;
	}
	std::reverse(write_buffer_.begin() + start_index, write_buffer_.begin() + write_buffer_offset_);
}

inline void IO::next_line(char* s) {
	if (current_char_ == '\0') {
		shift_char();
	}
	std::size_t index = 0;
	while (!eof_ && current_char_ != '\n') {
		s[index++] = current_char_;
		shift_char();
	}
	s[index] = 0;
	shift_char();
}

inline void IO::update_buffer() {
	if (read_buffer_offset_ == read_bytes_count_) {
		read_bytes_count_ = fread(&read_buffer_[0], sizeof(read_buffer_[0]), read_buffer_.size(), stdin);
		if (read_bytes_count_ == 0) {
			eof_ = true;
			return;
		}
		read_buffer_offset_ = 0;
	}
}

inline void IO::shift_char() {
	update_buffer();
	current_char_ = read_buffer_[read_buffer_offset_++];
}

inline int32_t IO::next_int() {
	return read_number<int32_t>();
}

inline uint32_t IO::next_uint() {
	return read_number<uint32_t>();
}

inline int64_t IO::next_long()  {
	return read_number<int64_t>();
}

inline uint64_t IO::next_ulong() {
	return read_number<uint64_t>();
}

inline char IO::next_char_ws() {
	if (current_char_ == '\0') {
		shift_char();
	}
	char ret = current_char_;
	shift_char();
	return ret;
}

inline char IO::next_char() {
	if (current_char_ == '\0') {
		shift_char();
	}
	skipws();
	char ret = current_char_;
	shift_char();
	return ret;
}

inline void IO::next_string(char* s) {
	if (current_char_ == '\0') {
		shift_char();
	}
	skipws();
	std::size_t index = 0;
	while (!eof_ && current_char_ != ' ' && current_char_ != '\n') {
		s[index++] = current_char_;
		shift_char();
	}
	s[index] = 0;
}

IO& IO::operator >>(int32_t& x) {
	x = ::next_int();
	return *this;
}

IO& IO::operator >>(uint32_t& x) {
	x = ::next_uint();
	return *this;
}

IO& IO::operator >>(int64_t& x) {
	x = ::next_long();
	return *this;
}

IO& IO::operator >>(uint64_t& x) {
	x = ::next_ulong();
	return *this;
}

IO& IO::operator >>(char& x) {
	x = ::next_char();
	return *this;
}

IO& IO::operator >>(char* s) {
	::next_string(s);
	return *this;
}

void IO::assign_files(const std::string& task) {
	if (useFastIO) {
		freopen((task + ".in").c_str(), "r", stdin);
		freopen((task + ".out").c_str(), "w", stdout);
	}
}

void IO::assign_files(const std::string& inputFile, const std::string& outputFile) {
	if (useFastIO) {
		freopen(inputFile.c_str(), "r", stdin);
		freopen(outputFile.c_str(), "w", stdout);
	}
}

void IO::assign_files_input_txt() {
	if (useFastIO) {
		freopen("input.txt", "r", stdin);
		freopen("output.txt", "w", stdout);
	}
}

inline void IO::flush() {
	fwrite(&write_buffer_[0], sizeof(write_buffer_[0]), write_buffer_offset_, stdout);
	write_buffer_offset_ = 0;
}

inline void IO::new_line() {
	if (write_buffer_offset_ == kBufferSize) {
		flush();
	}
	write_buffer_[write_buffer_offset_++] = '\n';
}

inline void IO::print_int(const int32_t x) {
	print_number(x);
}

inline void IO::print_uint(const uint32_t x) {
	print_number(x);
}

inline void IO::print_long(const int64_t x) {
	print_number(x);
}

inline void IO::print_ulong(const uint64_t x) {
	print_number(x);
}

inline void IO::print_char(const char x) {
	if (write_buffer_offset_ == kBufferSize) {
		flush();
	}
	write_buffer_[write_buffer_offset_++] = x;
}

inline void IO::print_string(const char* s) {
	for (std::size_t i = 0; s[i] != '\0'; ++i) {
		if (write_buffer_offset_ == kBufferSize) {
			flush();
		}
		write_buffer_[write_buffer_offset_++] = s[i];
	}
}

inline void IO::print_line(const char* s) {
	for (std::size_t i = 0; s[i] != '\0'; ++i) {
		if (write_buffer_offset_ == kBufferSize) {
			flush();
		}
		write_buffer_[write_buffer_offset_++] = s[i];
	}
	if (write_buffer_offset_ == kBufferSize) {
		flush();
	}
	write_buffer_[write_buffer_offset_++] = '\n';
}

IO& IO::operator <<(const int32_t x) {
	::print_int(x);
	return *this;
}

IO& IO::operator <<(const uint32_t x) {
	::print_uint(x);
	return *this;
}

IO& IO::operator <<(const int64_t x) {
	::print_long(x);
	return *this;
}

IO& IO::operator <<(const uint64_t x) {
	::print_ulong(x);
	return *this;
}

IO& IO::operator <<(const char x) {
	::print_char(x);
	return *this;
}

IO& IO::operator <<(const char* s) {
	::print_string(s);
	return *this;
}

IO& IO::operator <<(const std::string& s) {
	::print_string(s.c_str());
	return *this;
}

IO& IO::operator <<(std::ostream& (*)(std::ostream&)) {
	::new_line();
	return *this;
}

inline int32_t next_int() {
	if (useFastIO) {
		return io.next_int();
	}
	int32_t ret;
	*pin >> ret;
	return ret;
}

inline uint32_t next_uint() {
	if (useFastIO) {
		return io.next_uint();
	}
	uint32_t ret;
	*pin >> ret;
	return ret;
}

inline int64_t next_long() {
	if (useFastIO) {
		return io.next_long();
	}
	int64_t ret;
	*pin >> ret;
	return ret;
}

inline uint64_t next_ulong() {
	if (useFastIO) {
		return io.next_ulong();
	}
	uint64_t ret;
	*pin >> ret;
	return ret;
}

inline void skipws() {
	if (useFastIO) {
		io.skipws();
		return;
	}
	*pin >> std::noskipws;
	char ch;
	while (*pin >> ch) {
		if (ch != ' ' && ch != '\n') {
			pin->unget();
			break;
		}
	}
}

inline void skip_to_endl() {
	if (useFastIO) {
		io.skip_to_endl();
		return;
	}
	std::string s;
	std::getline(*pin, s);
}

inline char next_char_ws() {
	if (useFastIO) {
		return io.next_char_ws();
	}
	char ch;
	*pin >> std::noskipws >> ch >> std::skipws;
	return ch;
}

inline char next_char() {
	if (useFastIO) {
		return io.next_char();
	}
	char ch;
	*pin >> std::skipws >> ch;
	return ch;
}

inline void next_string(char* s) {
	if (useFastIO) {
		io.next_string(s);
		return;
	}
	*pin >> s;
}

void next_line(char* s) {
	if (useFastIO) {
		io.next_line(s);
		return;
	}
	std::string st;
	std::getline(*pin, st);
	strcpy(s, st.c_str());
}

inline void flush() {
	if (useFastIO) {
		io.flush();
		return;
	}
	*pout << std::flush;
}

inline void new_line() {
	if (useFastIO) {
		io.new_line();
		return;
	}
	*pout << std::endl;
}

inline void print_int(const int32_t x) {
	if (useFastIO) {
		io.print_int(x);
		return;
	}
	*pout << x;
}

inline void print_uint(const uint32_t x) {
	if (useFastIO) {
		io.print_uint(x);
		return;
	}
	*pout << x;
}

inline void print_long(const int64_t x) {
	if (useFastIO) {
		io.print_long(x);
		return;
	}
	*pout << x;
}

inline void print_ulong(const uint64_t x) {
	if (useFastIO) {
		io.print_ulong(x);
		return;
	}
	*pout << x;
}

inline void print_char(const char x) {
	if (useFastIO) {
		io.print_char(x);
		return;
	}
	*pout << x;
}

inline void print_string(const char* s) {
	if (useFastIO) {
		io.print_string(s);
		return;
	}
	*pout << s;
}

inline void print_line(const char* s) {
	if (useFastIO) {
		io.print_line(s);
		return;
	}
	*pout << s << std::endl;
}
