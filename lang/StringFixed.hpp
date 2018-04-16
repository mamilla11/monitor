#ifndef LANG_STRINGFIXED_HPP_
#define LANG_STRINGFIXED_HPP_

#include <cstring>
#include <string.h>
#include <cstddef>
#include <cstdint>

#include <iterator>
#include <algorithm>
#include <string>
#include <type_traits>

#include <lang/StringConst.hpp>

#if __cplusplus > 201103L
#include <experimental/string_view>
#endif

namespace lang {


namespace detail {

template<typename T>
struct no_sprintf_for_this_type;

// Default value is not valid (not compile for not specified type)
template<typename T>
constexpr lang::StringConst specifier = no_sprintf_for_this_type<T>::value;

template<>
constexpr lang::StringConst specifier<signed char> = "%d";

template<>
constexpr lang::StringConst specifier<unsigned char> = "%u";

template<>
constexpr lang::StringConst specifier<int> = "%d";

template<>
constexpr lang::StringConst specifier<unsigned int> = "%u";

template<>
constexpr lang::StringConst specifier<short int> = "%d";

template<>
constexpr lang::StringConst specifier<unsigned short int> = "%u";

template<>
constexpr lang::StringConst specifier<long int> = "%l";

template<>
constexpr lang::StringConst specifier<unsigned long int> = "%l";

template<>
constexpr lang::StringConst specifier<long long int> = "%l";

template<>
constexpr lang::StringConst specifier<unsigned long long int> = "%l";

template<>
constexpr lang::StringConst specifier<float> = "%g";

template<>
constexpr lang::StringConst specifier<double> = "%g";
}


template<std::size_t N>
class StringFixed
{
public:
    constexpr static std::size_t CAPACITY = N + 1;
    constexpr static std::size_t USEFUL_CAPACITY = N;

    using CharType = char;

    typedef CharType                                  value_type;
    typedef CharType&                                 reference;
    typedef const CharType&                           const_reference;
    typedef CharType*                                 pointer;
    typedef const CharType*                           const_pointer;
    typedef CharType*                                 iterator;
    typedef const CharType*                           const_iterator;
    typedef std::reverse_iterator<iterator>           reverse_iterator;
    typedef std::reverse_iterator<const_iterator>     const_reverse_iterator;
    typedef size_t                                    size_type;

    enum Status {
        OK,
        BAD_ALLOC,
        EMPTY
    };

    /******************************************
     * Constructors ***************************
     ******************************************/

    // With test
    StringFixed() :
        _length(1)
    {
        _buffer[0] = '\0';
    }

    // With test
    StringFixed(char c) :
        _length(2)
    {
        _buffer[0] = c;
        _buffer[1] = '\0';
    }

    // With test
    StringFixed(const char* s, std::size_t length) {
        set(s, length);
    }

    StringFixed(const char* s) {
        set_cstring(s);
    }

    // With test
    StringFixed(const StringConst& s) {
        set(s.data(), s.length());
    }

    // With test
    template<std::size_t str_length>
    StringFixed(const char (&s)[str_length]) :
        _length(str_length)
    {
        set(s);
    }

    // With test
    template<std::size_t M>
    StringFixed(StringFixed<M> const& src) :
        _length(0)
    {
        set(src);
    }

#if __cplusplus > 201103L
    // With test
    StringFixed(std::experimental::string_view const& src) {
        set(src.data(), src.length());
    }
#endif

    StringFixed(etl::string_view const& src) {
        set(src.data(), src.length());
    }

    // With test
    StringFixed(StringFixed const& src) :
        _length(0)
    {
        set(src.data(), src.length());
    }

    StringFixed(StringFixed&&) = default;

    /******************************************
     * Destructor *****************************
     ******************************************/

    ~StringFixed() = default;

    /******************************************
     * Operator= ******************************
     ******************************************/

    StringFixed& operator=(StringFixed&&) = default;

    // With test
    template<std::size_t str_length>
    StringFixed& operator=(const char (&s)[str_length]) {
        set(s);
        return *this;
    }

    // With test
    template<std::size_t M>
    StringFixed& operator=(StringFixed<M> const& src) {
        set(src.data(), src.length());
        return *this;
    }

#if __cplusplus > 201103L
    // With test
    StringFixed& operator=(std::experimental::string_view src) {  // ref is need to prevent
        set(src.data(), src.length());                            // using a c-string init
        return *this;
    }

    // With test
    operator std::experimental::string_view() const {
        return std::experimental::string_view(data(), length());
    }
#endif

    StringFixed& operator=(etl::string_view src) {  // ref is need to prevent
        set(src.data(), src.length());                            // using a c-string init
        return *this;
    }

    operator etl::string_view() const {
        return etl::string_view(data(), length());
    }

    /******************************************
     * set ************************************
     ******************************************/

    // With test
    StringFixed& operator=(StringFixed const& src) {
        set(src.data(), src.length());
        return *this;
    }

    // With test
    Status set(char c) {
        return set(_to_pointer(&c), 1);
    }

    // With test
    Status set_cstring(const char* s) {
        return set(s, _strlen_save(s, USEFUL_CAPACITY));
    }

    // With test
    template<std::size_t M>
    Status set(StringFixed<M> const& src) {
        return set(src.data(), src.length());
    }

#if __cplusplus > 201103L
    // With test
    Status set(std::experimental::string_view const& src) {
        return set(src.data(), src.length());
    }
#endif

    Status set(etl::string_view const& src) {
        return set(src.data(), src.length());
    }

    // With test
    Status set(const char* s, std::size_t length)
    {
        auto status = OK;
        if (length > USEFUL_CAPACITY) {
            status = BAD_ALLOC;
            length = USEFUL_CAPACITY;
        }

        _length = length + 1;
        std::copy(s, s + length, begin());
        _buffer[_useful_length()] = '\0';

        return status;
    }

    // With test
    template<std::size_t str_length>
    Status set(const char (&s)[str_length])
    {
        _length = str_length;

        auto status = OK;
        if (_length - 1 > USEFUL_CAPACITY) {
            status = BAD_ALLOC;
            _length = USEFUL_CAPACITY;
        }

        std::copy(s, s + _length, begin());
        _buffer[_useful_length()] = '\0';

        return status;
    }

    /******************************************
     * Adding stuff ***************************
     ******************************************/

    // With test
    template<std::size_t str_length>
    StringFixed& operator+=(const char (&s)[str_length]) {
        add(s);
        return *this;
    }

    // With test
    template<std::size_t M>
    StringFixed& operator+=(StringFixed<M> const& src) {
        add(src.data(), src.length());
        return *this;
    }

    // With test
    StringFixed& operator+=(StringConst const& src) {
        add(src.data(), src.length());
        return *this;
    }

#if __cplusplus > 201103L
    // With test
    StringFixed& operator+=(std::experimental::string_view const& src) {
        add(src.data(), src.length());
        return *this;
    }
#endif

    StringFixed& operator+=(etl::string_view const& src) {
        add(src.data(), src.length());
        return *this;
    }

    // With test
    Status add(char c)
    {
        return add(&c, 1);
    }

    // With test
    StringFixed<N>& operator<<(char c)
    {
        this->add(c);
        return *this;
    }

    // With test
    template<std::size_t str_length>
    Status add(const char (&s)[str_length])
    {
        return add(_to_cpointer(s), str_length - 1);
    }

    // With test
    template<std::size_t str_length>
    StringFixed<N>& operator<<(const char (&s)[str_length])
    {
        this->add(_to_cpointer(s), str_length - 1);
        return *this;
    }

    // With test
    Status add_cstring(const char* s)
    {
        return add(s, _strlen_save(s, USEFUL_CAPACITY));
    }

    // With test
    template<std::size_t M>
    Status add(StringFixed<M> const& src)
    {
        return add(src.begin(), src.length());
    }

    // With test
    template<std::size_t M>
    StringFixed<N>& operator<<(StringFixed<M> const& src)
    {
        this->add(src.begin(), src.length());
        return *this;
    }

    // With test
    Status add(StringConst src) {
        return add(src.data(), src.length());
    }

    // With test
    StringFixed<N>& operator<<(StringConst src)
    {
        this->add(src.data(), src.length());
        return *this;
    }

#if __cplusplus > 201103L
    // With test
    Status add(std::experimental::string_view const& src) {
        return add(src.data(), src.length());
    }

    // With test
    StringFixed<N>& operator<<(std::experimental::string_view const& src)
    {
        this->add(src.data(), src.length());
        return *this;
    }
#endif

    Status add(etl::string_view const& src) {
        return add(src.data(), src.length());
    }

    StringFixed<N>& operator<<(etl::string_view const& src)
    {
        this->add(src.data(), src.length());
        return *this;
    }

    // With test
    template<typename T>
    StringFixed<N>& operator<<(T val)
    {
        static_assert(std::is_fundamental<T>::value, "Primitive type required");
        uint32_t remain_length = CAPACITY - _useful_length();
        _length += snprintf(end(), remain_length, detail::specifier<T>, val);
        return *this;
    }

    // With test
    Status add(const char* s, std::size_t length)
    {
        if (_length + length - 1 > USEFUL_CAPACITY) {
            return BAD_ALLOC;
        }

        std::copy(s, s + length, end());
        _length += length;
        _buffer[_useful_length()] = '\0';

        return OK;
    }

    // With test
    Status pop_back() {
        if (_useful_length() > 0) {
            _length--;
            _buffer[_useful_length()] = '\0';

            return OK;
        }
        else {
            return EMPTY;
        }
    }

    /******************************************
     * Access stuff ***************************
     ******************************************/

    // With test
    const_reference back() {
        return *rbegin();
    }

    const_reference operator[](std::size_t i) {
        return _buffer[i];
    }

    const_pointer data() const {
        return _to_pointer(_buffer);
    }

    operator char*() {
        return _to_pointer(_buffer);
    }

    operator const char*() const {
        return _to_pointer(_buffer);
    }

    pointer data() {
        return _to_pointer(_buffer);
    }

    std::size_t length() const {
        return _length - 1;
    }

    iterator begin() {
        return _to_pointer(_buffer);
    }

    const_iterator begin() const {
        return _to_pointer(_buffer);
    }

    iterator end() {
        return &_buffer[_useful_length()];
    }

    const_iterator end() const {
        return &_buffer[_useful_length()];
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    /******************************************
     * Status stuff ***************************
     ******************************************/

    bool full() const {
        return _useful_length() == USEFUL_CAPACITY;
    }

    // With test
    bool empty() const {
        return _useful_length() == 0;
    }

    /******************************************
     * Compare stuff **************************
     ******************************************/

    template<std::size_t M>
    int compare(const StringFixed<M>& other) const {
        return _compare(this->begin(), this->end(), other.begin(), other.end());
    }

    int compare_cstring(const char* other) const {
        return _compare(this->begin(), this->end(),
                        other, other + _strlen_save(other, USEFUL_CAPACITY));
    }

    template<std::size_t str_length>
    int compare(const char (&other)[str_length]) const {
        return _compare(this->begin(), this->end(), other, other + str_length - 1);
    }

    int compare(const char* other, std::size_t length) const {
        return _compare(this->begin(), this->end(), other, other + length);
    }

    /******************************************
     * Other stuff ****************************
     ******************************************/

    void update_length()
    {
        _length = _strlen_save(_buffer, USEFUL_CAPACITY);
        if (_length != USEFUL_CAPACITY)
            _length += 1; // for '\0'
    }

    void clear() {
        _length = 1;
        _buffer[0] = '\0';
    }

private:
    CharType _buffer[CAPACITY];
    std::size_t _length;

    inline std::size_t _useful_length() const {
        return _length - 1;
    }

    template<typename T>
    inline pointer _to_pointer(T ptr) {
        return reinterpret_cast<pointer>(ptr);
    }

    template<typename T>
    inline const_pointer _to_pointer(T ptr) const {
        return reinterpret_cast<const_pointer>(ptr);
    }

    template<typename T>
    inline const_pointer _to_cpointer(T ptr) const {
        return reinterpret_cast<const_pointer>(ptr);
    }

    int _compare(const_pointer begin_iter1, const_pointer end_inter1,
                 const_pointer begin_iter2, const_pointer end_inter2) const
    {
        auto it1 = begin_iter1;
        auto it2 = begin_iter2;

        while ((it1 != end_inter1) && (it2 != end_inter2)) {
            ++it1;
            ++it2;
        }

        if ((it1 == end_inter1) && (it2 == end_inter2)) {
            return 0;
        }
        else if (it2 == end_inter2) {
            return -1;
        }
        else {
            return 1;
        }
    }

    static std::size_t _strlen_save(register const char *s, size_t maxlen)
    {
        register const char *e;
        std::size_t n;

        for (e = s, n = 0; *e && n < maxlen; e++, n++);

        return n;
    }
};

// With test
template<std::size_t N>
bool operator ==(const StringFixed<N>& lhs, const StringFixed<N>& rhs)
{
    return (lhs.length() == rhs.length()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

// With test
template<std::size_t N, std::size_t M>
bool operator ==(const StringFixed<N>& lhs, const StringFixed<M>& rhs)
{
    return (lhs.length() == rhs.length()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

// With test
template<std::size_t N, std::size_t M>
bool operator ==(const StringFixed<N>& lhs, const char (&rhs)[M])
{
    return (lhs.length() == std::strlen(rhs)) && std::equal(lhs.begin(), lhs.end(), rhs);
}

// With test
template<std::size_t N, std::size_t M>
bool operator ==(const char (&lhs)[M], const StringFixed<N>& rhs)
{
    return (rhs.length() == std::strlen(lhs)) && std::equal(rhs.begin(), rhs.end(), lhs);
}

// With test
template<std::size_t N>
bool operator !=(const StringFixed<N>& lhs, const StringFixed<N>& rhs)
{
    return !(lhs == rhs);
}

// With test
template<std::size_t N, std::size_t M>
bool operator !=(const StringFixed<N>& lhs, const StringFixed<M>& rhs)
{
    return !(lhs == rhs);
}

// With test
template<std::size_t N, std::size_t M>
bool operator !=(const StringFixed<N>& lhs, const char (&rhs)[M])
{
    return !(lhs == rhs);
}

// With test
template<std::size_t N, std::size_t M>
bool operator !=(const char (&lhs)[M], const StringFixed<N>& rhs)
{
    return !(lhs == rhs);
}

template<std::size_t N>
bool operator >(const StringFixed<N>& lhs, const StringFixed<N>& rhs)
{
    return (strncmp(lhs.data(),rhs.data(), N)>0);
}

template<std::size_t N>
bool operator <(const StringFixed<N>& lhs, const StringFixed<N>& rhs)
{
    return (rhs>lhs);
}

#if __cplusplus > 201103L
// With test
template<std::size_t N>
bool operator ==(const StringFixed<N>& lhs, const std::experimental::string_view& rhs)
{
    return (lhs.length() == rhs.length()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

// With test
template<std::size_t N>
bool operator ==(const std::experimental::string_view& lhs, const StringFixed<N>& rhs)
{
    return (lhs.length() == rhs.length()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<std::size_t N>
bool operator !=(const StringFixed<N>& lhs, const std::experimental::string_view& rhs)
{
    return !(lhs == rhs);
}

template<std::size_t N>
bool operator !=(const std::experimental::string_view& lhs, const StringFixed<N>& rhs)
{
    return !(lhs == rhs);
}
#endif

template<std::size_t N>
bool operator ==(const StringFixed<N>& lhs, const etl::string_view& rhs)
{
    return (lhs.length() == rhs.length()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<std::size_t N>
bool operator ==(const etl::string_view& lhs, const StringFixed<N>& rhs)
{
    return (lhs.length() == rhs.length()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<std::size_t N>
bool operator !=(const StringFixed<N>& lhs, const etl::string_view& rhs)
{
    return !(lhs == rhs);
}

template<std::size_t N>
bool operator !=(const etl::string_view& lhs, const StringFixed<N>& rhs)
{
    return !(lhs == rhs);
}


}  // namespace lang

#endif /* LANG_STRINGFIXED_HPP_ */
