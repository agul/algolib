#pragma once
#include "Head.h"
#include "Maths.h"
#include "StringUtils.h"

class IO {
public:
	constexpr static const int32_t IO_ERR = -1;
	constexpr static const std::size_t kBufferSize = 1 << 18;

	IO() : read_bytes_count_(0), read_buffer_offset_(0), current_char_(0), eof_(false) {
		static_assert(kBufferSize > 32, "Size of a buffer must be greater than 32 due to comparison in IO::flush() method.");
	}

	~IO() {
		flush();
	}

	IO(const IO&) = delete;
	IO& operator=(const IO&) = delete;
	IO(const IO&&) = delete;
	IO& operator=(const IO&&) = delete;

	bool eof() const;

	inline void shift_char();
	inline void skipws();
	inline void skip_to_endl();

	inline int32_t next_int();
	inline uint32_t next_uint();
	inline int64_t next_long();
	inline uint64_t next_ulong();
	inline char next_char_ws();
	inline char next_char();
	inline void next_string(char* s);
	inline void next_line(char* s);

	IO& operator >>(int32_t& x);
	IO& operator >>(uint32_t& x);
	IO& operator >>(int64_t& x);
	IO& operator >>(uint64_t& x);
	IO& operator >>(char& x);
	IO& operator >>(char* s);

	inline void flush();
	inline void new_line();

	inline void print_int(const int32_t x);
	inline void print_uint(const uint32_t x);
	inline void print_long(const int64_t x);
	inline void print_ulong(const uint64_t x);
	inline void print_char(const char ch);
	inline void print_string(const char* s);
	//inline void print_string(const std::string& s);
	inline void print_line(const char* s);
	//inline void print_line(const std::string& s);

	IO& operator <<(const int32_t x);
	IO& operator <<(const uint32_t x);
	IO& operator <<(const int64_t x);
	IO& operator <<(const uint64_t x);
	IO& operator <<(const char x);
	IO& operator <<(const char* s);
	IO& operator <<(const std::string& s);

	// Intended to use with std::endl.
	IO& operator <<(std::ostream& (*)(std::ostream&));

	static void assign_files(const std::string& task);
	static void assign_files(const std::string& inputFile, const std::string& outputFile);
	static void assign_files_input_txt();

private:
	using Buffer = std::array<char, kBufferSize>;
	Buffer read_buffer_;
	Buffer write_buffer_;
	std::size_t read_bytes_count_;
	std::size_t read_buffer_offset_;
	std::size_t write_buffer_offset_;
	bool eof_;
	char current_char_;

	inline void update_buffer();

	template<typename T>
	T read_number();

	template<typename T>
	void print_number(const T& value);

};

extern bool useFastIO;
extern std::istream * pin;
extern std::ostream * pout;
extern IO io;

inline int32_t next_int();
inline uint32_t next_uint();
inline int64_t next_long();
inline uint64_t next_ulong();

inline void skipws();
inline void skip_to_endl();

inline char next_char_ws();
inline char next_char();
inline void next_string(char* s);
void next_line(char* s);

inline void flush();
inline void new_line();

inline void print_int(const int32_t x);
inline void print_uint(const uint32_t x);
inline void print_long(const int64_t x);
inline void print_ulong(const uint64_t x);
inline void print_char(const char x);
inline void print_string(const char* s);
//inline void print_string(const std::string& s);
inline void print_line(const char* s);
//inline void print_line(const std::string& s);
