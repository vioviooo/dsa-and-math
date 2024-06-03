#ifndef PROGRAMMING_LANGUAGES_AND_METHODS_BIG_INTEGER_H
#define PROGRAMMING_LANGUAGES_AND_METHODS_BIG_INTEGER_H

#include <string>
#include <vector>
#include <iostream>
#include <allocator.h>
#include <allocator_guardant.h>

class big_integer final
{

private:

    void clear();

    void copy_from(
        big_integer const &other);

    void initialize_from(
        int const *digits,
        size_t digits_count);

    void initialize_from(
        std::vector<int> const &digits,
        size_t digits_count);
    
    void initialize_from(
        std::vector<uint> const &digits,
        size_t digits_count);

    void initialize_from(
        std::string const &value,
        size_t base);

private:

    static void print_byte(
        std::ostream &stream,
        unsigned char byte_value);

    static void dump_int_value(
        std::ostream &stream,
        int value);

public:

    void dump_value(
        std::ostream &stream) const
    {
        for (auto i = 0; i < get_digits_count(); ++i)
        {
            auto digit = get_digit(i);
            dump_int_value(stream, *reinterpret_cast<int *>(&digit));
            stream << ' ';
        }
    }

public: 
    // * choose your character!
    enum class multiplication_rule {
        karatsuba,
        trivial,
        schonhage_strassen
    };

    enum class division_rule {
        karatsuba,
        trivial,
        burnikel
    };

private:

    int _oldest_digit;
    uint* _other_digits;

private:

    big_integer &change_sign();

private: // * multiplication
    class multiplication {
        public:
            virtual ~multiplication() noexcept = default;
        
        public:
            virtual big_integer &multiply(
                big_integer &first_multiplier,
                big_integer const &second_multiplier) const = 0;
    };

    class trivial_multiplication final:
        public multiplication {
            public:
                big_integer &multiply(
                    big_integer &first_multiplier,
                    big_integer const &second_multiplier) const override;    
    };

    big_integer karatsuba(big_integer &first_multiplier, big_integer const &second_multiplier) const;

    // class karatsuba_multiplication final:
    //     public multiplication {
    //         public:
    //             big_integer &multiply(
    //                 big_integer &first_multiplier,
    //                 big_integer const &second_multiplier) const override;
    //         private:
    //             big_integer karatsuba(big_integer &x, big_integer const &y) const {
    //                 size_t n = std::max(x.get_digits_count(), y.get_digits_count());

    //                 // Base case for recursion
    //                 if (n <= 1) {
    //                     return multiply(x, y);
    //                 }

    //                 // n = (n / 2) + (n % 2); // Half size

    //                 // // Split the numbers into halves
    //                 // big_integer x_high = x.half_high(n);
    //                 // big_integer x_low = x.half_low(n);
    //                 // big_integer y_high = y.half_high(n);
    //                 // big_integer y_low = y.half_low(n);

    //                 // big_integer z0 = karatsuba(x_low, y_low);
    //                 // big_integer z2 = karatsuba(x_high, y_high);
    //                 // big_integer z1 = karatsuba(x_low + x_high, y_low + y_high) - z2 - z0;

    //                 // Shift z2 and z1 and sum them up
    //                 // return (z2.shift_left(2 * n) + z1.shift_left(n) + z0);
    //                 return *this;
    //             }

    // };
private: // * division
    class division {
        public:
            virtual ~division() noexcept = default;
        
        public:
            virtual big_integer &divide(
                big_integer &dividend,
                big_integer const &divisor) const = 0;
    };

    class trivial_division final:
        public division {
            public:
                big_integer &divide(
                    big_integer &dividend,
                    big_integer const &divisor) const override;    
    };

public:

    big_integer& multiply(
        big_integer &first_multiplier,
        big_integer const &second_multiplier,
        allocator *allocator,
        big_integer::multiplication_rule multiplication_rule);

    big_integer& divide(
        big_integer &dividend,
        big_integer const &divisor,
        allocator *allocator,
        big_integer::division_rule division_rule);
    inline int get_digits_count() const noexcept;

    std::string digit_to_string(uint digit);

    inline int sign() const noexcept;

    inline bool is_equal_to_zero() const noexcept;

    inline uint get_digit(
        int position) const noexcept;

    std::string bigint_to_string(big_integer const value) const;

public:

    big_integer(
        int const *digits,
        size_t digits_count);

    big_integer(
        std::vector<int> const &digits);

    big_integer(
        std::vector<uint> const &digits);

    big_integer(
        std::string const &value,
        size_t base);

    big_integer( // * base is automatically 10
        std::string const &value
    );

    big_integer(uint k);

public:

    big_integer(
        big_integer const &other);

    big_integer &operator=(
        big_integer const &other);

    ~big_integer();

public:

    big_integer &operator+=(
        big_integer const &other);

    big_integer operator+(
        big_integer const &other) const;

    big_integer &operator-=(
        big_integer const &other);

    big_integer operator-(
        big_integer const &other) const;

    big_integer operator-() const;

    big_integer &operator*=(
        big_integer const &other);

    big_integer operator*(
        big_integer const &other) const;

    big_integer &operator/=(
        big_integer const &other);

    big_integer operator/(
        big_integer const &other) const;

    big_integer &operator%=(
        big_integer const &other);

    big_integer operator%(
        big_integer const &other) const;

    bool operator==(
        big_integer const &other) const;

    bool operator!=(
        big_integer const &other) const;

    bool operator<(
        big_integer const &other) const;

    bool operator<=(
        big_integer const &other) const;

    bool operator>(
        big_integer const &other) const;

    bool operator>=(
        big_integer const &other) const;

    big_integer operator~() const;

public:

    big_integer &operator&=(
        big_integer const &other);

    big_integer operator&(
        big_integer const &other) const;

    big_integer &operator|=(
        big_integer const &other);

    big_integer operator|(
        big_integer const &other) const;

    big_integer &operator^=(
        big_integer const &other);

    big_integer operator^(
        big_integer const &other) const;

    big_integer &operator<<=(
        size_t shift_value);

    big_integer operator<<(
        size_t shift_value) const;

    big_integer &operator>>=(
        size_t shift_value);

    big_integer operator>>(
        size_t shift_value) const;

    friend std::ostream &operator<<(
        std::ostream &stream,
        big_integer const &value);

    friend std::istream &operator>>(
        std::istream &stream,
        big_integer &value);

    unsigned int get_digit_big_endian(int position) const;

};

#endif //PROGRAMMING_LANGUAGES_AND_METHODS_BIG_INTEGER_H