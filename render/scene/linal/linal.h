/*

    linal.h

Низкоуровневое описание математический объектов

*/

class Vec3 {
public:
    double x, y, z;
    Vec3();
    Vec3(double x, double y, double z);
    double abs() const;
    bool is_null() const;
    bool any_null() const;
    void normalize();
    Vec3 normalized();
    Vec3 operator+(Vec3 other);
    Vec3 operator-(Vec3 other);
    Vec3 operator/(double other);
    double operator[](int const& index) const;
};

Vec3 vv(const Vec3& a, const Vec3& b);
double dot(const Vec3& a, const Vec3& b);
Vec3 operator*(const Vec3& a, const double b);
Vec3 operator*(const double a, const Vec3& b);


//поля: точка отсчета, направление и минимальный и максимальный множители 
//на каторые нужно домножать вектор направления при поиске решения
// x и y - координаны зкрана соотв этому лучу 
class Ray {
public:
    Vec3 from;
    Vec3 dir;
    double min_q, max_q;
    int x, y;
    Ray(Vec3 from, Vec3 dir, double mi, double ma, double x_, double y_);
};

class Matr3 {
public:
    double body[3][3];
    Matr3();
    Matr3(Vec3 a, Vec3 b, Vec3 c);
    Matr3(double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33);
    void trans();
    Matr3 operator+(Matr3 other);
    Matr3 operator-(Vec3 other);
    Matr3 operator*(Matr3 other);
    Vec3 operator*(Vec3 other);
};

Vec3 dot(Matr3 m, Vec3 v);

