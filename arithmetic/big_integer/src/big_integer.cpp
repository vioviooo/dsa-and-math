#include <cstring>
#include <stdexcept>
#include <vector>

#include "big_integer.h"
#include <string>
#include <bitset>

#define pss std::pair<std::string, std::string> 
#define BASE_32 "4294967296" // * 2^32 
#define all(x) (x).begin(), (x).end()

int string_cmp(const std::string& num1, const std::string& num2) {
    bool is_negative1 = (num1[0] == '-');
    bool is_negative2 = (num2[0] == '-');

    std::string abs_num1 = is_negative1 ? num1.substr(1) : num1;
    std::string abs_num2 = is_negative2 ? num2.substr(1) : num2;

    if (is_negative1 and !is_negative2) return -1;
    if (!is_negative1 and is_negative2) return 1;

    int cmp_res;
    if (abs_num1.length() != abs_num2.length()) {
        cmp_res = (abs_num1.length() > abs_num2.length()) ? 1 : -1;
    } else {
        cmp_res = abs_num1.compare(abs_num2);
        if (cmp_res > 0) cmp_res = 1;
        if (cmp_res < 0) cmp_res = -1;
    }

    if (is_negative1 and is_negative2) {
        cmp_res = -cmp_res;
    }

    return cmp_res;
}

std::string string_add(std::string const &a, std::string const &b) 
{
    std::string result;

    int hyper_carry = 0, sum = 0; 

    int i = a.size() - 1, j = b.size() - 1;

    while (i >= 0 or j >= 0 or hyper_carry) {
        sum = hyper_carry + (i >= 0 ? a[i--] - '0' : 0) + (j >= 0 ? b[j--] - '0' : 0);
        hyper_carry = sum / 10;
        result.push_back(sum % 10 + '0');
    }

    std::reverse(all(result));

    return result;
}

std::string string_mult(const std::string &num1, const std::string &num2) {
    if (num1 == "0" or num2 == "0") {
        return "0";
    }

    std::vector <int> res_arr(num1.size() + num2.size(), 0);

    std::string num1_rev = std::string(num1.rbegin(), num1.rend());
    std::string num2_rev = std::string(num2.rbegin(), num2.rend());

    for (size_t i = 0; i < num1_rev.size(); ++i) {
        for (size_t j = 0; j < num2_rev.size(); ++j) {
            int mul = (num1_rev[i] - '0') * (num2_rev[j] - '0');
            res_arr[i + j] += mul;
            res_arr[i + j + 1] += res_arr[i + j] / 10;  
            res_arr[i + j] %= 10; 
        }
    }

    while (res_arr.size() > 1 and res_arr.back() == 0) {
        res_arr.pop_back();
    }

    std::string res_str;
    for (auto it = res_arr.rbegin(); it != res_arr.rend(); ++it) {
        res_str.push_back(*it + '0');
    }

    return res_str;
}
std::string string_pow(const std::string &base, uint exponent) {
    if (exponent == 0 and base == "0") {
        throw std::runtime_error("pow(0, 0) is undefined");
    }

    if (exponent == 0) {
        return "1";
    }

    if (base == "0") {
        return "0";
    }

    std::string result = "1";
    std::string curr_base = base;
    uint curr_exponent = exponent;

    // * fast exponentiation
    while (curr_exponent > 0) {
        if (curr_exponent % 2 == 1) {
            result = string_mult(result, curr_base);
        }
        curr_base = string_mult(curr_base, curr_base);
        curr_exponent /= 2;
    }

    return result;
}

int char_to_int(char c) {
    if (c >= '0' and c <= '9') {
        return c - '0';
    } else if (c >= 'A' and c <= 'Z') {
        return c - 'A' + 10;
    } else if (c >= 'a' and c <= 'z') {
        return c - 'a' + 10;
    }
    throw std::invalid_argument("Invalid character in number string");
}

std::string string_to_decimal(const std::string& number, int base) {
    if (base < 2 or base > 36) {
        throw std::invalid_argument("Base must be in the range [2...36]");
    }

    std::string result = "0";
    int power = 0;

    for (int i = number.size() - 1; i >= 0; --i) {
        int digitValue = char_to_int(number[i]);
        if (digitValue >= base) {
            throw std::invalid_argument("Invalid character in the number for the given base");
        }
        std::string tmp = string_mult(std::to_string(digitValue), string_pow(std::to_string(base), power));
        result = string_add(result, tmp);
        ++power;
    }

    return result;
}

std::string string_subtract(const std::string &num1, const std::string &num2) {
    std::string result;
    int carry = 0;

    int len1 = num1.size();
    int len2 = num2.size();

    for (int i = 0; i < len1; i++) {
        int digit1 = num1[len1 - 1 - i] - '0';
        int digit2 = (i < len2) ? num2[len2 - 1 - i] - '0' : 0;

        int curr = digit1 - digit2 - carry;

        if (curr < 0) {
            curr += 10;
            carry = 1;
        } else {
            carry = 0;
        }

        result.push_back(curr + '0');
    }

    while (result.size() > 1 and result.back() == '0') {
        result.pop_back();
    }

    std::reverse(all(result));
    return result;
}

pss string_divide(const std::string &num1, const std::string &num2) {
    if (num2 == "0") {
        throw std::invalid_argument("Division by zero.");
    }

    if (num1 == "0") {
        return make_pair("0", num2);
    }

    bool is_negative = (num1[0] == '-') ^ (num2[0] == '-');

    std::string dividend = (num1[0] == '-') ? num1.substr(1) : num1;
    std::string divisor = (num2[0] == '-') ? num2.substr(1) : num2;

    std::string result = "";

    std::string curr;

    for (char digit : dividend) {
        curr += digit;

        while (curr.size() > 1 and curr[0] == '0') {
            curr.erase(0, 1);
        }

        int quotient = 0;
        while (curr.size() > divisor.size() or 
              (curr.size() == divisor.size() and curr >= divisor)) {
            curr = string_subtract(curr, divisor);
            quotient++;
        }

        result += std::to_string(quotient);
    }

    while (result.size() > 1 and result[0] == '0') {
        result.erase(0, 1);
    }

    std::string n1 = (num1[0] == '-') ? num1.substr(1) : num1;
    std::string n2 = (num2[0] == '-') ? num2.substr(1) : num2;

    std::string rem = string_subtract(n1, string_mult(result, n2));

    if (is_negative and result != "0") {
        result = "-" + result;
        rem = "-" + rem;
    } else if (is_negative and result == "0") {
        rem = "-" + rem;
    }

    return make_pair(result, rem);
}

// * stirng operations region end

std::string big_integer::bigint_to_string(big_integer const value) const {
    std::string res = "0";

    if (value.is_equal_to_zero()) {
        return res;
    }

    if (value._other_digits == nullptr) { 
        if (value.sign() == -1) {
            big_integer b(value);
            b.change_sign();
            std::string res = bigint_to_string(b);
            return "-" + res;
        } else {
            return std::to_string(value._oldest_digit);
        }
    }

    if (*(value._other_digits) == 1) {
        if (value.sign() == -1) {
            big_integer b(value);
            b.change_sign();
            std::string res = bigint_to_string(b);
            return "-" + res;
        } else {
            return std::to_string(value._oldest_digit);
        }
    }

    int size = *(value._other_digits);

    uint i = 1;
    for (; i < size; ++i) {
        res = string_add(res, string_mult(std::to_string(value._other_digits[i]), string_pow(BASE_32, i - 1)));
    }

    if (value.sign() == -1) {
        int old_d = value._oldest_digit;
        old_d = old_d ^ (1 << ((sizeof(int) << 3) - 1));
        std::string mystr = std::to_string(old_d);
        res = string_add(res, string_mult(mystr, string_pow(BASE_32, i - 1)));
        res = "-" + res;
    } else {
        res = string_add(res, string_mult(std::to_string(value._oldest_digit), string_pow(BASE_32, i - 1)));
    }

    return res;
}

void big_integer::clear()
{
    _oldest_digit = 0;
    delete[] _other_digits;
    _other_digits = nullptr;
}

void big_integer::copy_from(
    big_integer const &other)
{
    _oldest_digit = other._oldest_digit;
    _other_digits = nullptr;
    if (other._other_digits == nullptr) {
        return;
    }

    _other_digits = new uint[*other._other_digits];
    std::memcpy(_other_digits, other._other_digits, sizeof(uint) * (*other._other_digits));
}

void big_integer::initialize_from(
    int const *digits,
    size_t digits_count)
{
    if (digits == nullptr) {
        throw std::logic_error("pointer to digits array must not be nullptr");
    }

    if (digits_count == 0) {
        throw std::logic_error("digits array length must be > 0");
    }

    _oldest_digit = digits[digits_count - 1];
    _other_digits = (digits_count == 1
        ? nullptr
        : new uint[digits_count]);

    if (_other_digits == nullptr) {
        return;
    }

    *_other_digits = (uint)digits_count;

    std::memcpy(_other_digits + 1, digits, sizeof(uint) * (digits_count - 1));
}

void big_integer::initialize_from(
    std::vector<uint> const &digits,
    size_t digits_count)
{
    _other_digits = nullptr;

    if (digits.empty() or digits_count == 0) {
        throw std::logic_error("std::vector<int> of digits should not be empty" + std::to_string(digits_count));
    }

    _oldest_digit = digits[digits_count - 1];

    if (digits_count == 1) {
        return;
    }

    _other_digits = new uint[digits_count];
    *_other_digits = digits_count;

    for (auto i = 0; i < digits_count - 1; ++i) {
        _other_digits[i + 1] = digits[i];
    }
}

void big_integer::initialize_from(
    std::vector<int> const &digits,
    size_t digits_count)
{
    _other_digits = nullptr;

    if (digits.empty() or digits_count == 0) {
        throw std::logic_error("std::vector<int> of digits should not be empty");
    }

    _oldest_digit = digits[digits_count - 1];

    if (digits_count == 1) {
        return;
    }

    _other_digits = new uint[digits_count];
    *_other_digits = digits_count;

    for (auto i = 0; i < digits_count - 1; ++i) {
        _other_digits[i + 1] = *reinterpret_cast<uint const *>(&digits[i]);
    }
}

void big_integer::initialize_from(
    std::string const &value,
    size_t base)
{
    if (base < 2 or base > 36) {
        throw std::logic_error("base must be in range [2..36]");
    }

    std::string val = value;

    if (value.empty()) {
        throw std::logic_error("value must not be empty");
    }

    while (val[0] == '0' and val.size() > 1) {
        val.erase(0, 1);
    }

    std::string st = val;
    if (base != 10) {
        st = string_to_decimal(val, base);
    }

    std::string curr = st;
    bool flag = false;
    if (st[0] == '-') {
        flag = true;
        this->change_sign();
        curr.assign(st, 1);
    }

    std::vector <uint> k_vec;
    
    while (curr != "0") {
        pss p = string_divide(curr, BASE_32);
        
        std::string whole = p.first;
        std::string rem = p.second; 

        k_vec.push_back(uint(std::stoull(rem)));
        curr = whole;
    }

    size_t sze = k_vec.size();

    _other_digits = nullptr;

    if (curr == "0" and sze == 0 ) {
        _oldest_digit = 0;
        return;
    }

    _oldest_digit = k_vec[sze - 1];
    if (sze == 1) {
        if (k_vec[sze - 1] > INT_MAX) { // ???? 
            _oldest_digit = 0;
            _other_digits = new uint[2];
            _other_digits[0] = 2;
            _other_digits[1] = k_vec[sze - 1];
        }
        return;
    }
    
    _other_digits = new uint[sze];
    *_other_digits = sze;

    for (auto i = 0; i < sze - 1; ++i) {
        _other_digits[i + 1] = *reinterpret_cast<uint const*>(&k_vec[i]);
    }
}

void big_integer::print_byte(
    std::ostream &stream,
    unsigned char byte_value)
{
    for (int i = 0; i < 8; i++) {
        stream << ((byte_value >> (7 - i)) & 1);
    }
}

void big_integer::dump_int_value(
    std::ostream &stream,
    int value)
{
    auto *p = (unsigned char *)&value;
    for (int i = 0; i < sizeof(int); i++) {
        print_byte(stream, *p++);
        stream << ' ';
    }
}

big_integer &big_integer::change_sign()
{
    _oldest_digit ^= (1 << ((sizeof(int) << 3) - 1));

    return *this;
}

inline int big_integer::get_digits_count() const noexcept
{
    return static_cast<int>(_other_digits == nullptr
         ? 1
         : *_other_digits);
}

inline int big_integer::sign() const noexcept
{
    if (is_equal_to_zero()) {
        return 0;
    }

    return 1 - (static_cast<int>((*reinterpret_cast<uint const *>(&_oldest_digit) >> ((sizeof(int) << 3) - 1))) << 1);
}

inline bool big_integer::is_equal_to_zero() const noexcept
{
    return _oldest_digit == 0 and _other_digits == nullptr;
}

inline uint big_integer::get_digit(
    int position) const noexcept
{
    if (_other_digits == nullptr) {
        return position == 0
            ? _oldest_digit
            : 0;
    }

    int const digits_count = get_digits_count();
    if (position < digits_count - 1) {
        return _other_digits[position + 1];
    }

    if (position == digits_count - 1) {
        return _oldest_digit;
    }

    return 0;
}

big_integer::big_integer(
    int const *digits,
    size_t digits_count)
{
    initialize_from(digits, digits_count);
}

big_integer::big_integer(std::vector<int> const &digits) {
    size_t digits_count = digits.size();
    _other_digits = nullptr;

    if (digits.empty() or digits_count == 0) {
        throw std::logic_error("std::vector<int> of digits should not be empty");
    }

    _oldest_digit = digits[digits_count - 1];

    if (digits_count == 1) {
        return;
    }

    _other_digits = new uint[digits_count];
    *_other_digits = digits_count;

    for (auto i = 0; i < digits_count - 1; ++i) {
        _other_digits[1 + i] = *reinterpret_cast<unsigned int const *>(&digits[i]);
    }
}

big_integer::big_integer(std::vector<uint> const &digits) {   
    int digits_count = digits.size();
    _oldest_digit = 0;

    _other_digits = nullptr;

    if (digits_count == 0) {
        throw std::logic_error("std::vector<int> of digits should not be empty!");
    }

    if (digits[digits_count - 1] < INT32_MAX) {
        _oldest_digit = digits[digits_count - 1];
        _other_digits = new unsigned int[digits_count];
        *_other_digits = digits_count;

        for (auto i = 0; i < digits_count; ++i) {
            _other_digits[1 + i] = *reinterpret_cast<unsigned int const *>(&digits[i]);
        }
        return;
    }
    _other_digits = new unsigned int[digits_count + 1];
    *_other_digits = digits_count + 1;

    for (auto i = 0; i < digits_count; ++i) {
        _other_digits[1 + i] = *reinterpret_cast<unsigned int const *>(&digits[i]);
    }
}

big_integer::big_integer(
    std::string const &value,
    size_t base)
{
    initialize_from(value, base);
}
big_integer::big_integer(std::string const &value) 
{
    initialize_from(value, 10);
    if (value[0] == '-') {
        change_sign();
    }
}

big_integer::big_integer(uint k)
{
    if (k < INT_MAX) { 
        _oldest_digit = k;
        _other_digits = nullptr;
        return;
    }
    _other_digits = new uint[2];
    _other_digits[0] = 2;
    _other_digits[1] = k; 
    _oldest_digit = 0;
}

big_integer::big_integer(
    big_integer const &other)
{
    copy_from(other);
}

big_integer &big_integer::operator=(
    big_integer const &other)
{
    if (this != &other) {
        clear();
        copy_from(other);
    }

    return *this;
}

big_integer::~big_integer()
{
    clear();
}

big_integer &big_integer::operator+=(
    big_integer const &other)
{
    if (other.is_equal_to_zero())
    {
        return *this;
    }

    if (is_equal_to_zero())
    {
        return *this = other;
    }

    if (sign() == -1)
    {
        change_sign();
        *this += -other;
        return change_sign();
    }

    if (other.sign() == -1)
    {
        return *this -= -other;
    }

    auto const fi_digs_count = get_digits_count();
    auto const se_digs_count = other.get_digits_count();
    auto const digits_count = std::max(fi_digs_count, se_digs_count);

    uint operation_result = 0;

    constexpr int shift = sizeof(uint) << 2;
    constexpr int mask = (1 << shift) - 1;

    std::vector<int> result_digits(digits_count + 1);

    for (int i = 0; i < digits_count; ++i)
    {
        uint fi_dig = get_digit(i);
        uint se_dig = other.get_digit(i);
        result_digits[i] = 0;

        for (int j = 0; j < 2; ++j)
        {
            operation_result += (fi_dig & mask) + (se_dig & mask);
            fi_dig >>= shift;
            se_dig >>= shift;
            *reinterpret_cast<uint *>(&result_digits[i]) |= ((operation_result & mask) << shift * j);
            operation_result >>= shift;
        }
    }

    auto result_digits_count = result_digits.size();

    if (operation_result == 1)
    {
        if ((*reinterpret_cast<uint *>(&result_digits[digits_count - 1]) >> ((sizeof(uint) << 3) - 1)) == 0)
        {
            *reinterpret_cast<uint *>(&result_digits[digits_count - 1]) |= (1u << ((sizeof(uint) << 3) - 1));
        }
        else
        {
            result_digits.back() = 1;
        }
    }
    else if ((*reinterpret_cast<uint *>(&result_digits[digits_count - 1]) >> ((sizeof(uint) << 3) - 1)) == 0)
    {
        --result_digits_count;
    }

    clear();
    initialize_from(result_digits, result_digits_count);

    return *this;
}

big_integer big_integer::operator+(
    big_integer const &other) const
{
    return big_integer(*this) += other;
}

big_integer &big_integer::operator-=( 
    big_integer const &other)
{
    if (other.is_equal_to_zero()) {
        return *this;
    }

    if (is_equal_to_zero()) {
        clear();
        // std::cout << bigint_to_string(other) << std::endl;
        copy_from(other);
        // std::cout << bigint_to_string(*this) << std::endl;
        change_sign();
        // std::cout << bigint_to_string(*this) << std::endl;
        return *this;
    }

    if (*this == other) { // * case a - a = 0
        big_integer tmp("0");

        clear();

        copy_from(tmp);

        return *this;
    }

    if (*this < other) {
        big_integer res(other);

        res -= *this;
        
        res.change_sign();
        
        clear();
        
        copy_from(res);

        return *this;
    }

    if (sign() == -1 and other.sign() != -1) { // * case -a - b = -(a + b)
        this->change_sign();

        *this += other;

        this->change_sign();

        return *this;
    }

    if (this->sign() != -1 and other.sign() == -1) { // * case a - -b = a + b
        big_integer tmp(other);

        tmp.change_sign();

        *this += tmp;

        return *this;
    }

    if (sign() == -1 and other.sign() == -1) { //* case -a - -b = b - a
        change_sign();

        big_integer tmp(other);

        tmp.change_sign();

        tmp -= *this;

        clear();
        
        copy_from(tmp);

        return *this;
    }

    int cnt1 = get_digits_count(), cnt2 = other.get_digits_count();

    int digits_count = std::max(cnt1, cnt2);

    std::vector <uint> result_digits(digits_count - 1);

    bool is_taken = false;
    for (int i = 0; i < digits_count - 1; ++i) {
        uint fi_dig = get_digit(i), se_dig = other.get_digit(i);
        if (is_taken) {
            fi_dig--;
        }

        if (fi_dig == se_dig) {
            result_digits[i] = 0;
            is_taken = false;
        } else {
            result_digits[i] = fi_dig - se_dig;
            if (fi_dig < se_dig) {
                is_taken = true;
            } else {
                is_taken = false;
            }
        }
    }

    int old_dig1 = _oldest_digit, old_dig2 = other._oldest_digit;

    if (is_taken and cnt1 > cnt2) {
        old_dig1--;
    } else if (cnt1 == cnt2 and is_taken) {
        old_dig1 -= (old_dig2 + 1);
    } else if (cnt1 == cnt2) {
        old_dig1 -= old_dig2;
    }

    clear();

    _other_digits = nullptr;
    _oldest_digit = old_dig1;

    _other_digits = new uint[digits_count];
    *_other_digits = digits_count;

    for (auto i = 0; i < digits_count - 1; ++i) {
        _other_digits[i + 1] = result_digits[i];
    }

    return *this;
}

big_integer big_integer::operator-(
    big_integer const &other) const
{
    return big_integer(*this) -= other;
}

big_integer big_integer::operator-() const
{
    return big_integer(*this).change_sign();
}

big_integer& big_integer::operator*=(
    big_integer const &other)
{   
    // * case 0 * a = 0
    if (other.is_equal_to_zero()) {
        return *this = other;
    }

    // * case a * 0 = 0
    if (is_equal_to_zero()) {
        return *this;
    }

    bool is_negative = false;

    if (sign() == -1 and other.sign() == -1) {
        change_sign();

        *this *= -other;

        return *this;
    }

    // * case -a * b = -(a * b)
    if (sign() == -1) {
        change_sign();

        *this *= other;

        return *this;
    }

    // * case a * -b = -(a * b)
    if (other.sign() == -1) {
        *this *= -other;

        change_sign();

        return *this;
    }

    int sze1 = get_digits_count(), sze2 = other.get_digits_count();

    // std::cout << "sizeof(uint): " << sizeof(uint) << std::endl;

    int shift = sizeof(uint) << 2; //* (4 * 2^2) = 16
    int mask = (1 << shift) - 1; //* (1 * 2^shift - 1)//* 1111 1111 1111 1111
    

    big_integer res("0");
    big_integer mult("0");

    for (int i = 0; i < sze1; i++) {
        uint quo_A = get_digit(i) >> shift; // * quotient
        uint rem_A = get_digit(i) & mask; // * remainder

        for (int j = 0; j < sze2; j++) {
            uint quo_B = other.get_digit(j) >> shift;
            uint rem_B = other.get_digit(j) & mask;

            mult = std::move(big_integer(std::to_string(rem_A * rem_B)));
            mult <<= ((sizeof(uint) << 2) * ((i + j) << 1)); // * (4 * 2^2) * 2 * (i + j)
            res += mult; // * addition in bigints

            mult = std::move(big_integer(std::to_string(quo_A * quo_B)));
            mult <<= ((sizeof(uint) << 2) * (((i + j) << 1) + 2));
            res += mult;

            mult = std::move(big_integer(std::to_string(rem_A * quo_B)));
            mult <<= ((sizeof(uint) << 2) * (((i + j) << 1) + 1));
            res += mult;

            mult = std::move(big_integer(std::to_string(quo_A * rem_B)));
            mult <<= ((sizeof(uint) << 2) * (((i + j) << 1) + 1));
            res += mult;
        }
    }

    clear();
    
    *this = res;

    return *this;
}


// !!!!!
// big_integer big_integer::karatsuba(big_integer &first_multiplier, big_integer const &second_multiplier) const {

//     if (first_multiplier.is_equal_to_zero() or second_multiplier.is_equal_to_zero()) {        
//         big_integer res = big_integer("0");
//         return res;
//     }
    
//     int sze = std::max(first_multiplier.get_digits_count(), second_multiplier.get_digits_count());

//     if (sze < 10) {
//         return first_multiplier *= second_multiplier;
//     }

//     sze = sze / 2 + sze % 2;

//     big_integer multiplier = big_integer(string_pow("10", sze));

//     big_integer b("0");
//     b = first_multiplier / multiplier;

//     big_integer a("0");
// 	a = first_multiplier - (b * multiplier);
	
//     big_integer d("0");
//     d = second_multiplier / multiplier;

//     big_integer sze_bi(sze);

//     big_integer c("0");
// 	c = second_multiplier - (d * sze_bi);

//     big_integer z0("0");
//     z0 = karatsuba(a, c);

//     big_integer sumres("0");
//     sumres = a + b;

//     big_integer z1("0");
//     z1 = karatsuba(sumres, c + d);

//     big_integer z2("0");
//     z2 = karatsuba(b, d);

//     big_integer mn("0");
//     std::string powres = string_pow("10", 2 * sze);

//     mn = big_integer(powres);

//     big_integer bb("0");
//     bb = z0 + (z1 - z0 - z2) * multiplier + z2 * mn;

//     // * z0 + ((z1 - z0 - z2) * multiplier) + (z2 * (long long)(pow(10, 2 * N)));

//     return bb;
// }   

// trivial_multiplication final:
//         public multiplication {
//             public:
//                 big_integer &multiply(
//                     big_integer &first_multiplier,
                    // big_integer const &second_multiplier) const override;  

// big_integer& big_integer::trivial_multiplication::multiply(
//                     big_integer &first_multiplier,
//                     big_integer const &second_multiplier) const override 
// {
//     return first_multiplier *= second_multiplier;
// }

// big_integer& big_integer::trivial_multiplication::multiply(
//     big_integer &first_multiplier,
//     big_integer const &second_multiplier) const override 
// {
//     return first_multiplier *= second_multiplier;
// }

big_integer& big_integer::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{
    if (!allocator) {
        if (multiplication_rule == big_integer::multiplication_rule::trivial) {
            first_multiplier *= second_multiplier;
        } else if (multiplication_rule == big_integer::multiplication_rule::karatsuba) {
            std::cout << "HAVE YOUR KARATSUBA~" << std::endl;
            first_multiplier = karatsuba(first_multiplier, second_multiplier);
        }
    }
    return *this;
}

big_integer big_integer::operator*(
    big_integer const &other) const
{
    return big_integer(*this) *= other;
}

uint find_coefficient(uint left_bound, uint right_bound, big_integer div, const big_integer& x) {

    double res1 = static_cast<double>(left_bound) / 2;
    double res2 = static_cast<double>(right_bound) / 2;
    uint mid = (res1 + res2);

    big_integer res(mid);

    big_integer finall = res * x;

    if (finall == div){
        return mid;
    } else if (finall > div) {
        return find_coefficient(left_bound, mid, div, x);
    } else {
        big_integer rem = div - finall;
        if (rem >= x) return find_coefficient(mid, right_bound, div, x);
        else {
            return mid;
        }
    }
}

uint big_integer::get_digit_big_endian(int position) const {
    if (_other_digits == nullptr) {
        return position == 0
            ? _oldest_digit
            : 0;
    }

    int const digits_count = get_digits_count();

    if (position == 0) {
        return _oldest_digit;
    }

    if (position < digits_count) {
        return _other_digits[_other_digits[0] - position];
    }

    return 0;
}

// !!!!!

big_integer big_integer::karatsuba(big_integer &first_multiplier, big_integer const &second_multiplier) const {

    if (first_multiplier.is_equal_to_zero() or second_multiplier.is_equal_to_zero()) {        
        big_integer res = big_integer("0");
        return res;
    }
    
    int sze = std::max(first_multiplier.get_digits_count(), second_multiplier.get_digits_count());

    if (sze < 10) {
        return first_multiplier *= second_multiplier;
    }

    sze = sze / 2 + sze % 2;

    big_integer multiplier = big_integer(string_pow("10", sze));

    big_integer b("0");
    b = first_multiplier / multiplier;

    big_integer a("0");
	a = first_multiplier - (b * multiplier);
	
    big_integer d("0");
    d = second_multiplier / multiplier;

    big_integer sze_bi(sze);

    big_integer c("0");
	c = second_multiplier - (d * sze_bi);

    big_integer z0("0");
    z0 = karatsuba(a, c);

    big_integer sumres("0");
    sumres = a + b;

    big_integer z1("0");
    z1 = karatsuba(sumres, c + d);

    big_integer z2("0");
    z2 = karatsuba(b, d);

    big_integer mn("0");
    std::string powres = string_pow("10", 2 * sze);

    mn = big_integer(powres);

    big_integer bb("0");
    bb = z0 + (z1 - z0 - z2) * multiplier + z2 * mn;

    // * z0 + ((z1 - z0 - z2) * multiplier) + (z2 * (long long)(pow(10, 2 * N)));

    return bb;
}   

big_integer &big_integer::operator/=(big_integer const &divisor) {
    if (divisor.is_equal_to_zero()) {
        throw std::logic_error("big_integer: division by zero");
    }

    if (is_equal_to_zero()) {
        clear();
        initialize_from("0", 10);
        return *this;
    }

    if (*this < divisor and sign() >= 0 and divisor.sign() >= 0) {
        clear();
        initialize_from("0", 10);
        return *this; 
    }

    if (*this < divisor and sign() < 0 and divisor.sign() < 0) {
        change_sign();

        big_integer pos_divisor(*this);
        pos_divisor.clear();
        pos_divisor.copy_from(divisor);
        pos_divisor.change_sign();
        
        *this /= pos_divisor; 
        return *this;
    }

    if (*this < divisor and sign() < 0 and divisor.sign() < 0) {
        // std::cout << "Im here \n";
        change_sign();

        big_integer pos_divisor("0");
        pos_divisor.clear();
        pos_divisor.copy_from(divisor);
        pos_divisor.change_sign();
        // std::cout << bigint_to_string(*this) << '\n';
        // std::cout << bigint_to_string(pos_divisor) << std::endl;

        return *this /= pos_divisor;
    }

    if (*this < divisor and sign() < 0 and divisor.sign() >= 0) {
        change_sign();
        if (*this > divisor) {
            clear();
            initialize_from("0", 10);
            return *this;
        }
    }

    if (sign() < 0 and divisor.sign() < 0) {
        change_sign();
        big_integer neg_divisor("0");
        neg_divisor.clear();
        neg_divisor.copy_from(divisor);
        neg_divisor.change_sign();

        *this /= neg_divisor;
        return *this;
    }

    if (sign() < 0) {
        change_sign();
        *this /= divisor;
        change_sign();
        return *this;
    }

    if (divisor.sign() < 0) {
        *this /= -divisor;
        change_sign();
        return *this;
    }

    int dividend_length = get_digits_count();
    int divisor_length = divisor.get_digits_count();

    std::cout << "LOLOLOL " << divisor_length << ' ' << dividend_length << '\n';

    std::vector<uint> intermediate(divisor_length + 1, 0); 
    std::vector<uint> quotient;

    int index = 0;
    for (int i = 0; i < divisor_length; i++) {
        intermediate[i] = get_digit_big_endian(i);
    }

    index = divisor_length;

    std::vector<uint> temp(intermediate.begin(), intermediate.end() - 1);

    std::cout << "LOLOLOL " << bigint_to_string(*this) << ' ' << bigint_to_string(divisor) << '\n';

    std::reverse(all(temp));

    for (auto elem : temp) {
        std::cout << elem << ' ';
    } std::cout << std::endl;

    big_integer current_value(temp);
    std::reverse(all(temp));

    if (current_value < divisor) {
        intermediate[index] = get_digit_big_endian(index);
        index++;
        std::reverse(intermediate.begin(), intermediate.end());
        current_value = big_integer(intermediate);
        std::reverse(intermediate.begin(), intermediate.end());
    }

    while (index <= dividend_length) {
        std::cout << "1\n";
        uint quotient_digit = find_coefficient(0, std::stol(BASE_32) - 1, current_value, divisor);
        quotient.push_back(quotient_digit);

        big_integer quotient_bi(quotient_digit);
        current_value -= (quotient_bi * divisor);
        for (int i = 0; i < current_value.get_digits_count(); i++) {
            intermediate[i] = current_value.get_digit_big_endian(i);
        }

        intermediate[current_value.get_digits_count()] = get_digit_big_endian(index);
        index++;

        std::vector<uint> new_vector(current_value.get_digits_count() + 1);
        
        std::copy(intermediate.begin(), intermediate.begin() + current_value.get_digits_count() + 1, new_vector.begin());
        
        std::reverse(all(new_vector));

        std::cout << "3 " << new_vector.size() << ' ' << new_vector[0]  << ' ' << new_vector[1]  << ' ' << new_vector[2] << '\n';

        big_integer new_current_value(new_vector);

        std::cout << "LORD HAVE MERCY \n";

        std::reverse(all(new_vector));

        std::cout << "2222\n";

        if (new_current_value < divisor) {
            if (index > dividend_length - 1) {
                std::vector<uint> final_result;
                for (uint digit : quotient) {
                    new_vector.push_back(digit);
                }
                std::reverse(quotient.begin(), quotient.end());

                std::cout <<  "quo size: " << quotient.size() << '\n';

                big_integer aboba(quotient);

                if (aboba * divisor + divisor <= *this) {
                    std::cout << "4\n";
                    final_result.push_back(0);
                    std::reverse(final_result.begin(), final_result.end());
                    std::cout << "5\n";
                    *this = big_integer(final_result);
                    return *this;
                } else {
                    std::cout << "6\n";
                    *this = big_integer(quotient);
                    return *this;
                }
            } else if (index > dividend_length) {
                std::reverse(quotient.begin(), quotient.end());
                std::cout << "7\n";
                *this = big_integer(quotient);
                return *this;
            }

            intermediate[current_value.get_digits_count() + 1] = get_digit_big_endian(index);
            index++;
            std::reverse(intermediate.begin(), intermediate.end());
            new_current_value = big_integer(intermediate);
            std::reverse(intermediate.begin(), intermediate.end());
        }
        current_value = new_current_value;

        std::cout << "2\n";
    }

    std::reverse(quotient.begin(), quotient.end());
    *this = big_integer(quotient);

    return *this;
}

big_integer& big_integer::divide(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule)
{
    if (allocator == nullptr) {
        if (division_rule == big_integer::division_rule::trivial) {
            dividend /= divisor;
        }
    }

    return *this;
}

big_integer big_integer::operator/(
    big_integer const &other) const
{
    return big_integer(*this) /= other;
}

big_integer &big_integer::operator%=(
    big_integer const &other)
{
    std::string num = bigint_to_string(*this);
    std::string denom = bigint_to_string(other);

    pss p = string_divide(num, denom);

    big_integer tmp(p.second);

    clear();
    copy_from(tmp);

    return *this;
}

big_integer big_integer::operator%(
    big_integer const &other) const
{
    return big_integer(*this) %= other;
}

bool big_integer::operator==(
    big_integer const &other) const
{
    return (string_cmp(big_integer::bigint_to_string(*this), big_integer::bigint_to_string(other)) == 0) ? 1 : 0;
}

bool big_integer::operator!=(
    big_integer const &other) const
{
    return (string_cmp(big_integer::bigint_to_string(*this), big_integer::bigint_to_string(other)) != 0) ? 1 : 0;
}

bool big_integer::operator<(
    big_integer const &other) const
{       
    // std::cout << " I COMPARE :: \n";
    return (string_cmp(big_integer::bigint_to_string(*this), big_integer::bigint_to_string(other)) == -1) ? 1 : 0;
    //     return (string_cmp(big_integer::bigint_to_string(*this), big_integer::bigint_to_string(other)) == -1) ? 1 : 0;

}

bool big_integer::operator<=(
    big_integer const &other) const
{
    return (string_cmp(big_integer::bigint_to_string(*this), big_integer::bigint_to_string(other)) != 1) ? 1 : 0;
}

bool big_integer::operator>(
    big_integer const &other) const
{
    return (string_cmp(big_integer::bigint_to_string(*this), big_integer::bigint_to_string(other)) != -1) ? 1 : 0;
}

bool big_integer::operator>=(
    big_integer const &other) const
{
    return (string_cmp(big_integer::bigint_to_string(*this), big_integer::bigint_to_string(other)) != -1) ? 1 : 0;
}

big_integer big_integer::operator~() const
{
    big_integer result(*this);
    result._oldest_digit = ~result._oldest_digit;
    for (int i = 0; i < result.get_digits_count(); ++i) {
        result._other_digits[i] = ~result._other_digits[i];
    }
    return result;
}

big_integer& big_integer::operator&=(
    big_integer const &other)
{
    size_t sze = std::max(this->get_digits_count(), other.get_digits_count());

    big_integer b("0");

    b.copy_from(*this);

    std::vector <int> v(sze, 0);

    for (int i = 0; i < sze; ++i) {
        auto d = b.get_digit(i);
        d &= other.get_digit(i);
        v[i] = d;
    }

    big_integer bb(v);

    clear();

    copy_from(bb);

    return *this;
}

big_integer big_integer::operator&(
    big_integer const &other) const
{
	return big_integer(*this) &= other;
}

big_integer &big_integer::operator|=( // ??
    big_integer const &other)
{
	size_t sze = std::max(this->get_digits_count(), other.get_digits_count());

    big_integer b("0");

    b.copy_from(*this);

    std::vector <int> v(sze, 0);

    for (int i = 0; i < sze; ++i) {
        auto d = b.get_digit(i);
        d |= other.get_digit(i);
        v[i] = d;
    }

    big_integer bb(v);

    clear();

    copy_from(bb);

    return *this;
}

big_integer big_integer::operator|( 
    big_integer const &other) const
{
	return big_integer(*this) |= other;
}

big_integer &big_integer::operator^=( 
    big_integer const &other)
{
    size_t sze = std::max(this->get_digits_count(), other.get_digits_count());

    big_integer b("0");

    b.copy_from(*this);

    std::vector <int> v(sze, 0);

    for (int i = 0; i < sze; ++i) {
        auto d = b.get_digit(i);
        d ^= other.get_digit(i);
        v[i] = d;
    }

    big_integer bb(v);

    clear();

    copy_from(bb);

    return *this;
}

big_integer big_integer::operator^( 
    big_integer const &other) const
{
	return big_integer(*this) ^= other;
}

big_integer &big_integer::operator<<=(
    size_t shift_value)
{
    if (is_equal_to_zero() or shift_value == 0) {
        return *this;
    }

    auto value_sign = sign();
    if (value_sign == -1) {
        change_sign();
    }

    auto const added_by_shift_at_other_digits_digits_count = shift_value / (sizeof(uint) << 3);
    shift_value %= (sizeof(uint) << 3);

    auto added_by_shift_at_oldest_digit_digits_count = 0;
    if (_oldest_digit != 0) {
        uint oldest_digit = *reinterpret_cast<uint *>(&_oldest_digit);
        int oldest_value_bit_index = 0;
        while (oldest_digit != 1) {
            oldest_digit >>= 1;
            ++oldest_value_bit_index;
        }

        if (oldest_value_bit_index + shift_value > (sizeof(int) << 3) - 2) {
            ++added_by_shift_at_oldest_digit_digits_count;
        }
    }

    if (added_by_shift_at_oldest_digit_digits_count != 0 or added_by_shift_at_other_digits_digits_count != 0) {
        auto const added_digits_count = added_by_shift_at_oldest_digit_digits_count + added_by_shift_at_other_digits_digits_count;

        if (_other_digits == nullptr) {
            _other_digits = new uint[added_digits_count + 1];
            *_other_digits = added_digits_count + 1;
            std::memset(_other_digits + 1, 0, sizeof(uint) * (added_digits_count - 1));
            if (added_by_shift_at_oldest_digit_digits_count != 0)
            {
                _other_digits[*_other_digits - 1] = _oldest_digit;
                _oldest_digit = 0;
            }
            else
            {
                _other_digits[*_other_digits - 1] = 0;
            }
        }
        else {
            auto *new_digits = new uint[added_digits_count + *_other_digits];
            std::memset(new_digits + 1, 0, sizeof(uint) * added_digits_count);
            if (added_by_shift_at_oldest_digit_digits_count != 0)
            {
                new_digits[added_digits_count + *_other_digits - 1] = _oldest_digit;
                _oldest_digit = 0;
            }
            std::memcpy(new_digits + 1 + added_by_shift_at_other_digits_digits_count, _other_digits + 1, sizeof(uint) * (*_other_digits - 1));
            *new_digits = *_other_digits + added_digits_count;

            clear();
            _other_digits = new_digits;
        }
    }

    if (shift_value != 0) {
        auto const digits_count = get_digits_count();
        uint part_to_move_to_next_digit = 0;
        for (auto i = 0; i < digits_count; ++i) {
            auto digit_value = get_digit(i);
            auto *digit_address = i == digits_count - 1
                ? reinterpret_cast<uint *>(&_oldest_digit)
                : _other_digits + 1 + i;
            *digit_address <<= shift_value;
            *digit_address |= part_to_move_to_next_digit;
            part_to_move_to_next_digit = digit_value >> ((sizeof(uint) << 3) - shift_value);
        }
    }

    if (value_sign == -1) {
        change_sign();
    }

    return *this;
}

big_integer big_integer::operator<<(
    size_t shift_value) const
{
    return big_integer(*this) <<= shift_value;
}

big_integer &big_integer::operator>>=(
    size_t shift_value)
{
    if (is_equal_to_zero() and shift_value == 0) {
        return *this;
    }

    auto value_sign = sign();
    if (value_sign == -1) {
        change_sign();
    }

    auto const removed_by_shift_at_other_digits_digits_count = shift_value / (sizeof(uint) << 3);
    shift_value %= (sizeof(uint) << 3);

    if (removed_by_shift_at_other_digits_digits_count > 0) {
        if (_other_digits == nullptr and removed_by_shift_at_other_digits_digits_count >= *_other_digits) {
            clear();
            return *this;
        } else {
            uint* new_digits = new uint[*_other_digits - removed_by_shift_at_other_digits_digits_count];
            std::memcpy(new_digits + 1, _other_digits + 1 + removed_by_shift_at_other_digits_digits_count,
                        sizeof(uint) * (*_other_digits - removed_by_shift_at_other_digits_digits_count - 1));
            *new_digits = *_other_digits - removed_by_shift_at_other_digits_digits_count;

            clear();
            _other_digits = new_digits;
        }
    }

    if (shift_value != 0) {
        auto const digits_count = get_digits_count();
        uint part_to_move_from_next_digit = 0;
        for (int i = digits_count - 1; i >= 0; --i) {
            auto *digit_address = i == digits_count - 1
                ? reinterpret_cast<uint *>(&_oldest_digit)
                : _other_digits + 1 + i;
            uint digit_value = *digit_address;
            *digit_address >>= shift_value;
            *digit_address |= part_to_move_from_next_digit;
            part_to_move_from_next_digit = digit_value << ((sizeof(uint) << 3) - shift_value);
        }
    }

    if (_other_digits != nullptr and _other_digits[1] == 0) {
        delete[] _other_digits;
        _other_digits = nullptr;
    }

    if (value_sign == -1) {
        change_sign();
    }

    return *this;
}

big_integer big_integer::operator>>(
    size_t shift_value) const
{
	return big_integer(*this) >>= shift_value;
}

std::string digit_to_string(uint digit) {
    if (digit == 0) {
        return "0";
    }

    std::string result;
    while (digit > 0) {
        result.push_back('0' + digit % 10);
        digit /= 10;
    }

    std::reverse(all(result));
    return result;
}

std::ostream &operator<<(
    std::ostream &stream,
    big_integer const &value)
{
    std::string res = value.bigint_to_string(value);
    stream << res;
    
    return stream;
}

std::istream &operator>>(
    std::istream &stream, 
    big_integer &value)
{
    std::string input;
    stream >> input;

    value.initialize_from(input, 10);

    return stream;
}
