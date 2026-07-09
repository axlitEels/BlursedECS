#ifndef AUX_HPP_
#define AUX_HPP_

struct Vec3 {
    float x, y, z;

    Vec3& operator+=(const Vec3& rhs);

    friend Vec3 operator+(Vec3 lhs, const Vec3& rhs);

    friend bool operator==(const Vec3& lhs, const Vec3& rhs);
};

#endif // AUX_HPP_
