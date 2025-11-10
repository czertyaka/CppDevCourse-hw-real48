#include <cstdint>

namespace math {

#pragma pack(push, 1)
class Real48 {
public:
    // constructors
    constexpr Real48() noexcept;
    Real48(const float number);
    Real48(const double number);
    constexpr Real48(const Real48& o) = default;

    // conversion operators
    operator float() const;
    operator double() const noexcept;

    // assignment operators
    Real48& operator=(const Real48& b) noexcept = default;
    Real48& operator+=(const Real48& b);
    Real48& operator-=(const Real48& b);
    Real48& operator*=(const Real48& b);
    Real48& operator/=(const Real48& b);

    // arithmetic operators
    Real48 operator+() const noexcept;
    Real48 operator-() const noexcept;
    Real48 operator+(const Real48& o) const;
    Real48 operator-(const Real48& o) const;
    Real48 operator*(const Real48& o) const;
    Real48 operator/(const Real48& o) const;

    // comparison operators
    bool operator>(const Real48& o) const noexcept;
    bool operator<(const Real48& o) const noexcept;

    // classify
    enum class Class {
        NORMAL,
        ZERO
    };
    Class Classify() const noexcept;

    // limits
    consteval static Real48 min();
    consteval static Real48 max();
    consteval static Real48 epsilon();

private:
    consteval Real48(
        const std::uint8_t exponent,
        const std::uint64_t fraction,
        const std::uint8_t significand
    );
    
    std::uint8_t exponent_;
    std::uint64_t fraction_ : 39;
    std::uint8_t significand_ : 1;
};
#pragma pack(pop)

static_assert(sizeof(Real48) == 6);

}
