




class Light_source{
public:
    
    Light_source();
    virtual double get_intense(Vec3 camera, Vec3 normal, int alpha) = 0;
    virtual double intense() = 0;
protected:  
    inline double get_intence_in(const Vec3 camera, const Vec3 normal, const Vec3& direction, int alpha);
};

class Ambient_light: public Light_source{
protected:
    double intens;
public: 
    Ambient_light(double inte);
    double intense();
    double get_intense(Vec3 camera, Vec3 normal, int alpha);
};

class Point_light: public Light_source{
protected:    
    Vec3 pos;
    double intens;
public:
    Point_light(Vec3 pos, double inte);
    double intense();
    double get_intense(Vec3 camera, Vec3 normal, int alpha);
};

class Directed_liht: public Light_source{
protected:
    Vec3 direction;
    double intens;
public:
    Directed_liht(Vec3 dir, double inte);
    double intense();
    double get_intense(Vec3 camera, Vec3 normal, int alpha);
};

class Light{
    std::Vec3<Light_source*> light;
    double sum_intense;
public:
    Light();

    ~Light();

    bool push_back(Light_source* source);
    double get_light(Vec3 camera, Vec3 point, Vec3 normal, int alpha = -1);
};
