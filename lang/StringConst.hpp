#ifndef LANG_STRINGCONST_HPP_
#define LANG_STRINGCONST_HPP_

#if __cplusplus > 201103L
#include <experimental/string_view>
#endif

#include <etl/string_view.h>

namespace lang {

class StringConst
{
public:
	template<std::size_t N>
	constexpr StringConst(const char (&s)[N]) :
		_ptr(s), _size(N - 1)
	{ }

	constexpr const char& operator[](std::size_t i) {
		return _ptr[i];
	}

	constexpr const char& operator[](std::size_t i) const {
		return _ptr[i];
	}

	constexpr const char* data() {
		return _ptr;
	}

    constexpr const char* data() const {
        return _ptr;
    }

	constexpr std::size_t length() {
		return _size;
	}

    constexpr std::size_t length() const {
        return _size;
    }

    constexpr operator const char*() {
    	return _ptr;
    }

    constexpr operator const char*() const {
    	return _ptr;
    }

    constexpr const char* begin() const {
        return _ptr;
    }

    constexpr const char* end() const {
        return _ptr + _size;
    }

#if __cplusplus > 201103L
    operator std::experimental::string_view() const {
        return std::experimental::string_view(data(), length());
    }
#endif

    operator etl::string_view() const {
        return etl::string_view(data(), length());
    }

private:
	const char* const _ptr;
	const std::size_t _size;
};


}  // namespace lang

#endif /* LANG_STRINGCONST_HPP_ */
