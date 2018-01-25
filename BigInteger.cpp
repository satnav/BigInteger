#include "BigInteger.h"

#include <stdexcept>

// -------------------------------------------------------------------------------------

BigInteger::BigInteger()
    : BigInteger(0)
{
    // Do nothing
}

// -------------------------------------------------------------------------------------

BigInteger::BigInteger(const char* string)
    : BigInteger(std::string(string))
{
    // Do nothing
}

// -------------------------------------------------------------------------------------

BigInteger::BigInteger(const std::string& value)
{
    if (value.empty())
    {
        mDigits.push_back(0);
    }
    else
    {
        for (size_t i = 0; i < value.size(); i++)
        {
            if (std::isdigit(value[i]))
            {
                if (value[i] == '0')
                {
                    if (!mDigits.empty())
                    {
                        mDigits.insert(mDigits.begin(), 0);
                    }
                }
                else
                {
                    mDigits.insert(mDigits.begin(), value[i] - '0');
                }
            }
            else if (value[i] == '-' && i == 0)
            {
                mIsNegative = true;
            }
        }

        if (mDigits.empty())
        {
            *this = 0;
        }

        if (*this == 0)
        {
            mIsNegative = false;
        }
    }
}

// -------------------------------------------------------------------------------------

template <class T>
BigInteger::BigInteger(T value)
{
    // Check if negative number
    value = (mIsNegative = value < 0) ? -value : value;

    // Add new digits, even if value is 0   
    do
    {
        mDigits.push_back(value % 10);
        value /= 10;
    }
    while (value != 0);
}

// Explicit instantiations
template BigInteger::BigInteger<char>(char);
template BigInteger::BigInteger<unsigned char>(unsigned char);
template BigInteger::BigInteger<unsigned short>(unsigned short);
template BigInteger::BigInteger<unsigned int>(unsigned int);
template BigInteger::BigInteger<unsigned long>(unsigned long);
template BigInteger::BigInteger<unsigned long long>(unsigned long long);
template BigInteger::BigInteger<signed char>(signed char);
template BigInteger::BigInteger<signed short>(signed short);
template BigInteger::BigInteger<signed int>(signed int);
template BigInteger::BigInteger<signed long>(signed long);
template BigInteger::BigInteger<signed long long>(signed long long);

// -------------------------------------------------------------------------------------

BigInteger BigInteger::operator+() const
{
    return *this;
}

// -------------------------------------------------------------------------------------

BigInteger BigInteger::operator-() const
{
    BigInteger temp(*this);
    temp.mIsNegative = !temp.mIsNegative;
    return temp;
}

// -------------------------------------------------------------------------------------

BigInteger& BigInteger::operator+=(const BigInteger& rhs)
{
    if (mIsNegative == rhs.mIsNegative)
    {
        // Resize the vector
        const size_t lhsSize = mDigits.size();
        const size_t rhsSize = rhs.mDigits.size();
        if (lhsSize < rhsSize)
        {
            mDigits.resize(rhsSize);
        }

        // Add the digits
        Digit carry = 0;
        for (size_t i = 0; i < rhsSize; ++i)
        {
            carry = mDigits[i] + rhs.mDigits[i] + carry;
            mDigits[i] = carry % 10;
            carry /= 10;
        }

        // If there is still a remainder to account for
        for (size_t i = rhsSize; carry != 0; i++)
        {
            if (i >= mDigits.size())
            {
                mDigits.resize(i + 1);
            }
            carry = mDigits[i] + carry;
            mDigits[i] = carry % 10;
            carry /= 10;
        }
    }
    else if (mIsNegative)
    {
        *this = rhs - -(*this);
    }
    else
    {
        *this -= -rhs;
    }

    return *this;
}

// -------------------------------------------------------------------------------------

BigInteger& BigInteger::operator-=(const BigInteger& rhs)
{
    if (!mIsNegative && !rhs.mIsNegative)
    {
        // Only do subtraction if the result is guaranteed to be >= 0
        if (*this >= rhs)
        {
            for (size_t i = 0; i < rhs.mDigits.size(); i++)
            {
                Digit d1 = mDigits[i];
                Digit d2 = rhs.mDigits[i];

                // Borrow if need to
                if (d1 < d2)
                {
                    // Search until find a place to borrow
                    size_t j = i;
                    while (mDigits[++j] == 0);

                    // Borrow and transfer back
                    --mDigits[j];
                    while (--j > i)
                    {
                        // All digits in between must have been 0
                        mDigits[j] = 9;
                    }

                    // Add borrowed amount
                    d1 += 10;
                }

                // Do subtraction
                mDigits[i] = d1 - d2;
            }

            // Remove trailing zeros
            size_t size = mDigits.size();
            while (size > 1 && mDigits[size - 1] == 0)
            {
                --size;
            }
            mDigits.resize(size);
        }
        else
        {
            *this = -(rhs - *this);
        }
    }
    else if (!mIsNegative && rhs.mIsNegative)
    {
        *this += -rhs;
    }
    else if (mIsNegative && !rhs.mIsNegative)
    {
        *this = -(-(*this) + rhs);
    }
    else
    {
        *this = -rhs - -(*this);
    }

    return *this;
}

// -------------------------------------------------------------------------------------

BigInteger& BigInteger::operator*=(const BigInteger& rhs)
{
    // BigInteger to store result
    BigInteger total;

    // Perform multiplication
    for (size_t i = 0; i < mDigits.size(); i++)
    {
        // Check for zero
        const Digit d1 = mDigits[i];
        if (d1 == 0)
        {
            continue;
        }

        for (size_t j = 0; j < rhs.mDigits.size(); j++)
        {            
            // Check for zero
            const Digit d2 = rhs.mDigits[j];
            if (d2 == 0)
            {
                continue;
            }

            // Perform multiplication
            BigInteger temp = d1 * d2;
            temp <<= (i + j);
            total += temp;
        }
    }

    // Set the sign after everything has been added
    total.mIsNegative = (mIsNegative != rhs.mIsNegative);

    // Assign and return
    *this = total;
    return *this;
}

// -------------------------------------------------------------------------------------

BigInteger& BigInteger::operator/=(const BigInteger& rhs)
{
    *this = Divide(*this, rhs).first;
    return *this;
}

// -------------------------------------------------------------------------------------

BigInteger& BigInteger::operator%=(const BigInteger& rhs)
{
    *this = Divide(*this, rhs).second;
    return *this;
}

// -------------------------------------------------------------------------------------

BigInteger& BigInteger::operator<<=(size_t rhs)
{
    if (*this != 0)
    {
        mDigits.insert(mDigits.begin(), rhs, 0);
    }
    return *this;
}

// -------------------------------------------------------------------------------------

BigInteger& BigInteger::operator>>=(size_t rhs)
{
    if (rhs < mDigits.size())
    {
        mDigits.erase(mDigits.begin(), mDigits.begin() + rhs);
    }
    else
    {
        *this = 0;
    }
    return *this;
}

// -------------------------------------------------------------------------------------

BigInteger operator+(BigInteger lhs, const BigInteger& rhs)
{
    lhs += rhs;
    return lhs;
}

// -------------------------------------------------------------------------------------

BigInteger operator-(BigInteger lhs, const BigInteger& rhs)
{
    lhs -= rhs;
    return lhs;
}

// -------------------------------------------------------------------------------------

BigInteger operator*(BigInteger lhs, const BigInteger& rhs)
{
    lhs *= rhs;
    return lhs;
}

// -------------------------------------------------------------------------------------

BigInteger operator/(BigInteger lhs, const BigInteger& rhs)
{
    lhs /= rhs;
    return lhs;
}

// -------------------------------------------------------------------------------------

BigInteger operator%(BigInteger lhs, const BigInteger& rhs)
{
    lhs %= rhs;
    return lhs;
}

// -------------------------------------------------------------------------------------

BigInteger operator<<(BigInteger lhs, size_t rhs)
{
    lhs <<= rhs;
    return lhs;
}

// -------------------------------------------------------------------------------------

BigInteger operator>>(BigInteger lhs, size_t rhs)
{
    lhs >>= rhs;
    return lhs;
}

// -------------------------------------------------------------------------------------


bool operator==(const BigInteger& lhs, const BigInteger& rhs)
{
    return (lhs.mIsNegative == rhs.mIsNegative) && (lhs.mDigits == rhs.mDigits);
}

// -------------------------------------------------------------------------------------

bool operator!=(const BigInteger& lhs, const BigInteger& rhs)
{
    return !(lhs == rhs);
}

// -------------------------------------------------------------------------------------

bool operator<(const BigInteger& lhs, const BigInteger& rhs)
{
    if (lhs.mIsNegative != rhs.mIsNegative)
    {
        return lhs.mIsNegative;
    }
    else // both same sign.
    {
        // Test sizes
        const size_t lhsSize = lhs.mDigits.size();
        const size_t rhsSize = rhs.mDigits.size();
        if (lhsSize != rhsSize)
        {
            return (lhsSize < rhsSize) == !lhs.mIsNegative;
        }
        else
        {
            for (size_t i = 0; i < lhsSize; i++)
            {
                // Get digits
                const BigInteger::Digit lhsDigit = lhs.mDigits[lhsSize - i - 1];
                const BigInteger::Digit rhsDigit = rhs.mDigits[lhsSize - i - 1];

                // Compare
                if (lhsDigit != rhsDigit)
                {
                    return lhsDigit < rhsDigit;
                }
            }

            // Must be equal
            return false;
        }
    }
}

// -------------------------------------------------------------------------------------

bool operator>(const BigInteger& lhs, const BigInteger& rhs)
{
    return rhs < lhs;
}

// -------------------------------------------------------------------------------------

bool operator<=(const BigInteger& lhs, const BigInteger& rhs)
{
    return !(lhs > rhs);
}

// -------------------------------------------------------------------------------------

bool operator>=(const BigInteger& lhs, const BigInteger& rhs)
{
    return !(lhs < rhs);
}

// -------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& lhs, const BigInteger& rhs)
{
    lhs << rhs.ToString();
    return lhs;
}

// -------------------------------------------------------------------------------------

std::istream& operator>>(std::istream& lhs, BigInteger& rhs)
{
    std::string num;
    std::getline(lhs, num);
    rhs = BigInteger(num);
}

// -------------------------------------------------------------------------------------

std::string BigInteger::ToString() const
{
    // String to store result
    std::string s(mIsNegative ? "-" : "");
    
    // Add digits to string
    const size_t size = mDigits.size();
    for (size_t i = 0; i < size; i++)
    {
        s += std::to_string(mDigits[size - i - 1]);
    }

    return s;
}

// -------------------------------------------------------------------------------------

std::vector<BigInteger::Digit> BigInteger::GetDigits() const
{
    return mDigits;
}

// -------------------------------------------------------------------------------------

std::pair<BigInteger, BigInteger> BigInteger::Divide(const BigInteger& lhs, const BigInteger& rhs)
{
    if (rhs == 0)
    {
        throw std::logic_error("Divide by zero exception.");
    }

    BigInteger tempRhs = rhs.mIsNegative ? -rhs : rhs;
    BigInteger result;
    BigInteger temp;

    size_t i = 0;
    size_t lhsSize = lhs.mDigits.size();
    while (i < lhsSize)
    {
        while (temp < tempRhs)
        {
            if (i < lhsSize)
            {
                temp <<= 1;
                temp.mDigits[0] = lhs.mDigits[lhsSize - 1 - i];
                ++i;
                result <<= 1;
                // temp.mDigits.push_back(lhs.mDigits[lhsSize - 1 - i]);
            }
            else
            {
                break;
            }
        }

        // result <<= 1;
        // result.mDigits.push_back(0);
        while (tempRhs <= temp)
        {
            temp -= tempRhs;
            ++result.mDigits[0];
        }
    }

    result.mIsNegative = lhs.mIsNegative != rhs.mIsNegative;
    temp.mIsNegative = lhs.mIsNegative;
    return std::pair<BigInteger, BigInteger>(result, temp);
}
