#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <print>

#include "real48.hpp"

namespace math
{

namespace
{

#pragma pack(push, 1)
struct Binary64
{
    std::uint64_t fraction : 52;
    std::uint16_t exponent : 11;
    std::uint8_t significand : 1;
};
#pragma pack(pop)

static_assert(sizeof(Binary64) == sizeof(double));

} // namespace

constexpr Real48::Real48() noexcept : exponent_(0u), fraction_(), significand_()
{
}

Real48::Real48(const float number) : Real48(double{number})
{
}

Real48::Real48(const double number)
{
    switch (std::fpclassify(number))
    {
    case FP_INFINITE:
    case FP_NAN:
    {
        std::stringstream err{};
        std::print(err, "{} is either NaN or infinite", number);
        throw std::overflow_error(err.str());
        break;
    }
    case FP_ZERO:
    case FP_SUBNORMAL:
    {
        significand_ = 0u;
        fraction_ = {};
        exponent_ = 0u;
        break;
    }
    case FP_NORMAL:
    {
        Binary64 binary64{};
        std::memcpy(&binary64, &number, sizeof(number));

        const std::uint16_t exponent = binary64.exponent - 1023 + 129;
        if (exponent & 0xFF00)
        {
            std::stringstream err{};
            std::print(err, "{} can not be represented in Real48", number);
            throw std::overflow_error(err.str());
        }

        const std::uint64_t fraction = binary64.fraction >> 13;

        fraction_ = fraction;
        exponent_ = exponent;
        significand_ = binary64.significand;
        break;
    }
    default:
    {
        std::stringstream err{};
        std::print(err, "Can't determine class of {}", number);
        throw std::overflow_error(err.str());
    }
    }
}

Real48::operator float() const
{
    const double d = double{*this};

    if (std::fabs(d) > std::numeric_limits<float>::max() ||
        std::fabs(d) < std::numeric_limits<float>::min())
    {
        std::stringstream err{};
        std::print(err, "{} can't be converted to float", d);
        throw std::overflow_error(err.str());
    }

    return static_cast<float>(d);
}

Real48::operator double() const noexcept
{
    Binary64 binary64{};
    binary64.exponent = decltype(Binary64::exponent){exponent_}
        + 1023 - 129;
    binary64.fraction = decltype(Binary64::fraction){fraction_} << 13;
    binary64.significand = significand_;

    double result{};
    std::memcpy(&result, &binary64, sizeof(result));

    return result;
}

Real48& Real48::operator+=(const Real48& o)
{
    (*this) = double{*this} + double{o};
    return *this;
}

Real48& Real48::operator-=(const Real48& o)
{
    (*this) = double{*this} - double{o};
    return *this;
}

Real48& Real48::operator*=(const Real48& o)
{
    (*this) = double{*this} * double{o};
    return *this;
}

Real48& Real48::operator/=(const Real48& o)
{
    (*this) = double{*this} / double{o};
    return *this;
}

Real48 Real48::operator+() const noexcept
{
    return Real48{0.0} + *this;
}

Real48 Real48::operator-() const noexcept
{
    return Real48{0.0} - *this;
}

Real48 Real48::operator+(const Real48& o) const
{
    return double{*this} + double{o};
}

Real48 Real48::operator-(const Real48& o) const
{
    return double{*this} - double{o};
}

Real48 Real48::operator*(const Real48& o) const
{
    return double{*this} * double{o};
}

Real48 Real48::operator/(const Real48& o) const
{
    return double{*this} / double{o};
}

bool Real48::operator>(const Real48& o) const noexcept
{
    return double{*this} > double{o};
}

bool Real48::operator<(const Real48& o) const noexcept
{
    return double{*this} < double{o};
}

Real48::Class Real48::Classify() const noexcept
{
    if (exponent_ == 0u)
    {
        return Class::ZERO;
    }
    else
    {
        return Class::NORMAL;
    }
}

consteval Real48::Real48(const std::uint8_t exponent,
    const std::uint64_t fraction, const std::uint8_t significand)
    : exponent_(exponent), fraction_(fraction), significand_(significand)
{
}

consteval Real48 Real48::min()
{
    return {1u, 0u, 0u};
}

consteval Real48 Real48::max()
{
    return {255u, 0x7FFFFFFFFF, 0u};
}

consteval Real48 Real48::epsilon()
{
    return {90u, 0u, 0u};
}


} // namespace math
