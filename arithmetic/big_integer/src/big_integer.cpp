#include <cstring>
#include <stdexcept>
#include <vector>

#include "big_integer.h"

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
    if (other._other_digits == nullptr)
    {
        return;
    }

    _other_digits = new unsigned int[*other._other_digits];
    std::memcpy(_other_digits, other._other_digits, sizeof(unsigned int) * (*other._other_digits));
}

void big_integer::initialize_from(
    int const *digits,
    size_t digits_count)
{
    if (digits == nullptr)
    {
        throw std::logic_error("pointer to digits array must not be nullptr");
    }

    if (digits_count == 0)
    {
        throw std::logic_error("digits array length must  be GT 0");
    }

    _oldest_digit = digits[digits_count - 1];
    _other_digits = (digits_count == 1
        ? nullptr
        : new unsigned int[digits_count]);

    if (_other_digits == nullptr)
    {
        return;
    }

    *_other_digits = (unsigned int)digits_count;

    std::memcpy(_other_digits + 1, digits, sizeof(unsigned int) * (digits_count - 1));
}

void big_integer::initialize_from(
    std::vector<int> const &digits,
    size_t digits_count)
{
    _other_digits = nullptr;

    if (digits.empty() || digits_count == 0)
    {
        throw std::logic_error("std::vector<int> of digits should not be empty");
    }

    _oldest_digit = digits[digits_count - 1];

    if (digits_count == 1)
    {
        return;
    }

    _other_digits = new unsigned int[digits_count];
    *_other_digits = digits_count;

    for (auto i = 0; i < digits_count - 1; ++i)
    {
        _other_digits[i + 1] = *reinterpret_cast<unsigned int const *>(&digits[i]);
    }
}

void big_integer::initialize_from( // !!!
    std::string const &value,
    size_t base)
{
    if (base < 2 || base > 36)
    {
        throw std::logic_error("base must be in range [2..36]");
    }

    if (value.empty())
    {
        throw std::logic_error("value must not be empty");
    }

    _other_digits = nullptr;

    _oldest_digit = value[value.size() - 1];

    if (value.size() == 1)
    {
        return;
    }

    _other_digits = new unsigned int[value.size()];
    *_other_digits = value.size();
   
    for (auto i = 0; i < value.size() - 1; ++i)
    {
       _other_digits[i + 1] = value[i] - '0';
    }

    // for (auto i = 0; i < value.size(); ++i)
    // {
    //     std::cout << _other_digits[i] << std::endl;
    // }
}

void big_integer::print_byte(
    std::ostream &stream,
    unsigned char byte_value)
{
    for (int i = 0; i < 8; i++)
    {
        stream << ((byte_value >> (7 - i)) & 1);
    }
}

void big_integer::dump_int_value(
    std::ostream &stream,
    int value)
{
    auto *p = (unsigned char *)&value;
    for (int i = 0; i < sizeof(int); i++)
    {
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
    if (is_equal_to_zero())
    {
        return 0;
    }

    return 1 - (static_cast<int>((*reinterpret_cast<unsigned int const *>(&_oldest_digit) >> ((sizeof(int) << 3) - 1))) << 1);
}

inline bool big_integer::is_equal_to_zero() const noexcept
{
    return _oldest_digit == 0 && _other_digits == nullptr;
}

inline unsigned int big_integer::get_digit(
    int position) const noexcept
{
    if (_other_digits == nullptr)
    {
        return position == 0
            ? _oldest_digit
            : 0;
    }

    int const digits_count = get_digits_count();
    if (position < digits_count - 1)
    {
        return _other_digits[position + 1];
    }

    if (position == digits_count - 1)
    {
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

big_integer::big_integer(
    std::vector<int> const &digits)
{   
    if (digits.size() == 1) {
        initialize_from(digits, digits.size());
    } else {
        auto it = digits.begin();
        while (it != digits.end() && *it == 0) {
            ++it;
        }

        std::vector<int> cleaned_digits(it, digits.end());

        initialize_from(cleaned_digits, cleaned_digits.size());
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
}

big_integer::big_integer(
    big_integer const &other)
{
    copy_from(other);
}

big_integer &big_integer::operator=(
    big_integer const &other)
{
    if (this != &other)
    {
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

    auto const first_value_digits_count = get_digits_count();
    auto const second_value_digits_count = other.get_digits_count();
    auto const digits_count = std::max(first_value_digits_count, second_value_digits_count);

    unsigned int operation_result = 0;

    constexpr int shift = sizeof(unsigned int) << 2;
    constexpr int mask = (1 << shift) - 1;

    std::vector<int> result_digits(digits_count + 1);

    for (int i = 0; i < digits_count; ++i)
    {
        unsigned int first_value_digit = get_digit(i);
        unsigned int second_value_digit = other.get_digit(i);
        result_digits[i] = 0;

        for (int j = 0; j < 2; ++j)
        {
            operation_result += (first_value_digit & mask) + (second_value_digit & mask);
            first_value_digit >>= shift;
            second_value_digit >>= shift;
            *reinterpret_cast<unsigned int *>(&result_digits[i]) |= ((operation_result & mask) << shift * j);
            operation_result >>= shift;
        }
    }

    result_digits.back() += *reinterpret_cast<int *>(&operation_result);

    auto *maybe_overflowed_digit_ptr = reinterpret_cast<unsigned int *>(&*(result_digits.end() - 2));
    if (*maybe_overflowed_digit_ptr >> ((sizeof(unsigned int) << 3) - 1))
    {
        *maybe_overflowed_digit_ptr ^= (1 << ((sizeof(unsigned int) << 3) - 1));
        ++result_digits.back();
    }

    auto result_digits_count = result_digits.size();
    if (result_digits.back() == 0)
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
    return *this -= other;
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

big_integer &big_integer::operator*=(
    big_integer const &other)
{
    return *this *= other;
}

big_integer big_integer::operator*(
    big_integer const &other) const
{
    return big_integer(*this) *= other;
}

big_integer &big_integer::operator/=(
    big_integer const &other)
{
    return *this /= other;
}

big_integer big_integer::operator/(
    big_integer const &other) const
{
    return big_integer(*this) /= other;
}

big_integer &big_integer::operator%=(
    big_integer const &other)
{
    return *this %= other;
}

big_integer big_integer::operator%(
    big_integer const &other) const
{
    return big_integer(*this) %= other;
}

bool big_integer::operator==(
    big_integer const &other) const
{
    if (get_digits_count() != other.get_digits_count())
    {
        return false;
    }

    if (sign() != other.sign())
    {
        return false;
    }

    if (_oldest_digit != other._oldest_digit) 
    {
        return false;
    }

    for (int i = 0; i < get_digits_count(); ++i) {
        if (get_digit(i) != other.get_digit(i)) {
            return false;
        }
    }
    return true;
}

bool big_integer::operator!=(
    big_integer const &other) const
{
	return !(*this == other);
}

bool big_integer::operator<(
    big_integer const &other) const
{
	return (*this < other);
}

bool big_integer::operator<=(
    big_integer const &other) const
{
	return (*this <= other);
}

bool big_integer::operator>(
    big_integer const &other) const
{
	return (*this > other);
}

bool big_integer::operator>=(
    big_integer const &other) const
{
	return (*this >= other);
}

big_integer big_integer::operator~() const
{
    big_integer result(*this);
    result._oldest_digit = ~result._oldest_digit;
    for (int i = 0; i < result.get_digits_count(); ++i)
    {
        result._other_digits[i] = ~result._other_digits[i];
    }
    return result;
}

big_integer &big_integer::operator&=(
    big_integer const &other)
{
    return *this &= other;
}

big_integer big_integer::operator&(
    big_integer const &other) const
{
	return big_integer(*this) &= other;
}

big_integer &big_integer::operator|=(
    big_integer const &other)
{
	return *this |= other;
}

big_integer big_integer::operator|(
    big_integer const &other) const
{
	return big_integer(*this) |= other;
}

big_integer &big_integer::operator^=( // ?? 
    big_integer const &other)
{
    return *this ^= other;
}

big_integer big_integer::operator^( // ??
    big_integer const &other) const
{
	return big_integer(*this) ^= other;
}

big_integer &big_integer::operator<<=(
    size_t shift_value)
{
    if (is_equal_to_zero() || shift_value == 0)
    {
        return *this;
    }

    auto value_sign = sign();
    if (value_sign == -1)
    {
        change_sign();
    }

    auto const added_by_shift_at_other_digits_digits_count = shift_value / (sizeof(unsigned int) << 3);
    shift_value %= (sizeof(unsigned int) << 3);

    auto added_by_shift_at_oldest_digit_digits_count = 0;
    if (_oldest_digit != 0)
    {
        unsigned int oldest_digit = *reinterpret_cast<unsigned int *>(&_oldest_digit);
        int oldest_value_bit_index = 0;
        while (oldest_digit != 1)
        {
            oldest_digit >>= 1;
            ++oldest_value_bit_index;
        }

        if (oldest_value_bit_index + shift_value > (sizeof(int) << 3) - 2)
        {
            ++added_by_shift_at_oldest_digit_digits_count;
        }
    }

    if (added_by_shift_at_oldest_digit_digits_count != 0 || added_by_shift_at_other_digits_digits_count != 0)
    {
        auto const added_digits_count = added_by_shift_at_oldest_digit_digits_count + added_by_shift_at_other_digits_digits_count;

        if (_other_digits == nullptr)
        {
            _other_digits = new unsigned int[added_digits_count + 1];
            *_other_digits = added_digits_count + 1;
            std::memset(_other_digits + 1, 0, sizeof(unsigned int) * (added_digits_count - 1));
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
        else
        {
            auto *new_digits = new unsigned int[added_digits_count + *_other_digits];
            std::memset(new_digits + 1, 0, sizeof(unsigned int) * added_digits_count);
            if (added_by_shift_at_oldest_digit_digits_count != 0)
            {
                new_digits[added_digits_count + *_other_digits - 1] = _oldest_digit;
                _oldest_digit = 0;
            }
            std::memcpy(new_digits + 1 + added_by_shift_at_other_digits_digits_count, _other_digits + 1, sizeof(unsigned int) * (*_other_digits - 1));
            *new_digits = *_other_digits + added_digits_count;

            clear();
            _other_digits = new_digits;
        }
    }

    if (shift_value != 0)
    {
        auto const digits_count = get_digits_count();
        unsigned int part_to_move_to_next_digit = 0;
        for (auto i = 0; i < digits_count; ++i)
        {
            auto digit_value = get_digit(i);
            auto *digit_address = i == digits_count - 1
                ? reinterpret_cast<unsigned int *>(&_oldest_digit)
                : _other_digits + 1 + i;
            *digit_address <<= shift_value;
            *digit_address |= part_to_move_to_next_digit;
            part_to_move_to_next_digit = digit_value >> ((sizeof(unsigned int) << 3) - shift_value);
        }
    }

    if (value_sign == -1)
    {
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
    return *this >>= shift_value;
}

big_integer big_integer::operator>>(
    size_t shift_value) const
{
	return big_integer(*this) >>= shift_value;
}

std::string digit_to_string(unsigned int digit) {
    if (digit == 0) {
        return "0";
    }

    std::string result;
    while (digit > 0) {
        result.push_back('0' + digit % 10);
        digit /= 10;
    }

    std::reverse(result.begin(), result.end());
    return result;
}

// TODO:
std::ostream &operator<<(
    std::ostream &stream,
    big_integer const &value)
{
    if (value.is_equal_to_zero()) {
        return stream << '0';
    }

    if (value.sign() == -1) {
        stream << '-';
    }

    stream << value._oldest_digit;

    for (auto i = 0; i < *(value._other_digits) - 1; ++i) {
        stream << value._other_digits[i];
    }

    return stream;
}

// TODO::
std::istream &operator>>( // !!!
    std::istream &stream,
    big_integer &value)
{

    return stream;
}