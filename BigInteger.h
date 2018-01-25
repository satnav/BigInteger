#pragma once

// Make sure the machine supports 2's complement negative numbers
static_assert(-1 == ~0, "This machine does not support 2's complement representation.");

#include <cstdint>
#include <istream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

/// A number that can represent a large number of digits.
class BigInteger
{
public:
    /// The type of each digit.
    using Digit = uint8_t;

    /// Constructs a new BigInteger object with the default value 0.
    BigInteger();

    /// Constructs a new BigInteger object from a string.
    /// @param[in] value The string value to construct from.
    BigInteger(const char* string);

    /// Constructs a new BigInteger object from a string.
    /// @param[in] value The string value to construct from.
    BigInteger(const std::string& value);

    /// Constructs a new BigInteger object with the specified value.
    /// @param[in] value The initial value.
    template <class T> BigInteger(T value);

    /// Copy constructs a new BigInteger object.
    /// @param[in] bi The BigInteger object to copy.
    BigInteger(const BigInteger& bi) = default;

    /// Move constructs a new BigInteger object.
    /// @param[in] bi The BigInteger object to move.
    BigInteger(BigInteger&& bi) = default;

    /// Copy assigns a BigInteger object.
    /// @param[in] bi The BigInteger object to copy.
    /// @return A reference to the result.
    BigInteger& operator=(const BigInteger& bi) = default;

    /// Move assigns a BigInteger object.
    /// @param[in] bi The BigInteger object to move.
    /// @return A reference to the result.
    BigInteger& operator=(BigInteger&& bi) = default;

    /// Performs unary plus on the BigInteger object.
    /// @return A copy of the BigInteger object.
    BigInteger operator+() const;

    /// Performs unary minus on the BigInteger object.
    /// @return A copy of the negative of the BigInteger object.
    BigInteger operator-() const;

    /// Adds the right BigInteger to this one.
    /// @param[in] rhs The right operand.
    /// @return This BigInteger.
    BigInteger& operator+=(const BigInteger& rhs);

    /// Subtracts the right BigInteger from this one.
    /// @param[in] rhs The right operand.
    /// @return This BigInteger.
    BigInteger& operator-=(const BigInteger& rhs);

    /// Multiplies the right BigInteger with this one.
    /// @param[in] rhs The right operand.
    /// @return This BigInteger.
    BigInteger& operator*=(const BigInteger& rhs);

    /// Divides this BigInteger by the right one.
    /// @param[in] rhs The right operand.
    /// @return This BigInteger.
    BigInteger& operator/=(const BigInteger& rhs);

    /// Calculates the remainder of this BigInteger by the right one.
    /// @param[in] rhs The right operand.
    /// @return This BigInteger.
    BigInteger& operator%=(const BigInteger& rhs);

    /// Shifts all the digits to the left.
    /// @param[in] rhs The number of places to shift.
    /// @return This BigInteger.
    BigInteger& operator<<=(size_t rhs);

    /// Shifts all the digits to the right.
    /// @param[in] rhs The number of places to shift.
    /// @return This BigInteger.
    BigInteger& operator>>=(size_t rhs);

    /// Adds two BigInteger objects.
    /// @param[in] lhs The left operand.
    /// @param[in] rhs The right operand.
    friend BigInteger operator+(BigInteger lhs, const BigInteger& rhs);

    /// Subtracts two BigInteger objects.
    /// @param[in] lhs The left operand.
    /// @param[in] rhs The right operand.
    friend BigInteger operator-(BigInteger lhs, const BigInteger& rhs);

    /// Multiplies two BigInteger objects.
    /// @param[in] lhs The left operand.
    /// @param[in] rhs The right operand.
    friend BigInteger operator*(BigInteger lhs, const BigInteger& rhs);

    /// Divides two BigInteger objects.
    /// @param[in] lhs The left operand.
    /// @param[in] rhs The right operand.
    friend BigInteger operator/(BigInteger lhs, const BigInteger& rhs);

    /// Calculates the modulus from two BigInteger objects.
    /// @param[in] lhs The left operand.
    /// @param[in] rhs The right operand.
    friend BigInteger operator%(BigInteger lhs, const BigInteger& rhs);

    /// Shifts all the digits to the left.
    /// @param[in] lhs The object to shift.
    /// @param[in] rhs The number of places to shift.
    /// @return A new BigInteger.
    friend BigInteger operator<<(BigInteger lhs, size_t rhs);

    /// Shifts all the digits to the right.
    /// @param[in] lhs The object to shift.
    /// @param[in] rhs The number of places to shift.
    /// @return A new BigInteger.
    friend BigInteger operator>>(BigInteger lhs, size_t rhs);

    /// Compares two BigInteger objects for equality.
    /// @param[in] lhs The left operand.
    /// @param[in] rhs The right operand.
    /// @return Whether they are equal.
    friend bool operator==(const BigInteger& lhs, const BigInteger& rhs);

    /// Compares two BigInteger objects for inequality.
    /// @param[in] lhs The left operand.
    /// @param[in] rhs The right operand.
    /// @return Whether they are not equal.
    friend bool operator!=(const BigInteger& lhs, const BigInteger& rhs);

    /// Determines whether the left BigInteger is less than the right.
    /// @param[in] lhs The left operand.
    /// @param[in] rhs The right operand.
    /// @return Whether the left is less than the right.
    friend bool operator<(const BigInteger& lhs, const BigInteger& rhs);

    /// Determines whether the left BigInteger is greater than the right.
    /// @param[in] lhs The left operand.
    /// @param[in] rhs The right operand.
    /// @return Whether the left is greater than the right.
    friend bool operator>(const BigInteger& lhs, const BigInteger& rhs);

    /// Determines whether the left BigInteger is less than or equal to the right.
    /// @param[in] lhs The left operand.
    /// @param[in] rhs The right operand.
    /// @return Whether the left is less than or equal to the right.
    friend bool operator<=(const BigInteger& lhs, const BigInteger& rhs);

    /// Determines whether the left BigInteger is greater than or equal to the right.
    /// @param[in] lhs The left operand.
    /// @param[in] rhs The right operand.
    /// @return Whether the left is greater than or equal to the right.
    friend bool operator>=(const BigInteger& lhs, const BigInteger& rhs);

    /// Prints the BigInteger to the output stream.
    /// @param[in] lhs The output stream to print to.
    /// @param[in] rhs The BigInteger to print.
    /// @return The output stream used.
    friend std::ostream& operator<<(std::ostream& lhs, const BigInteger& rhs);

    /// Reads a BigInteger object from the input stream.
    /// @param[in] lhs The input stream to read from.
    /// @param[in] rhs The BigInteger to store in.
    /// @return The input stream used.
    friend std::istream& operator>>(std::istream& lhs, BigInteger& rhs);

    /// Converts the BigInteger object to a string.
    /// @return The object as a string.
    std::string ToString() const;

    /// Returns the vector containing the digits.
    /// @return The digits.
    std::vector<Digit> GetDigits() const;

private:
    /// Holds all the digits of the number.
    std::vector<Digit> mDigits;

    /// Whether the number is negative
    bool mIsNegative = false;

    /// Divides two numbers to get the result and remainder.
    /// @param[in] lhs The left operand.
    /// @param[in] rhs The right operand.
    /// @return The result and the remainder.
    std::pair<BigInteger, BigInteger> Divide(const BigInteger& lhs, const BigInteger& rhs);
};
