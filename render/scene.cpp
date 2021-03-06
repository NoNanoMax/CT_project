/*

    scene.cpp

Программный файл для работы сцены

*/
#include <thread>
#include <vector>
#include <stdexcept>
#include <errno.h>
#include <string.h>
#include "math.h"
#include "scene.h"

// ------------------------------- camera -------------------------------

Matr3 Camera::calculate_rotate() {
    Matr3 a(cos(angles[0]), -sin(angles[0]),    0,
            sin(angles[0]), cos(angles[0]),     0,
            0,              0,                  1);
    Matr3 b(1, 0,              0,
            0, cos(angles[1]), -sin(angles[1]),
            0, sin(angles[1]), cos(angles[1]));
    Matr3 c(cos(angles[2]), -sin(angles[2]),    0,
            sin(angles[2]), cos(angles[2]),     0,
            0,              0,                  1);
    return a * b * c;
}


Ray Camera::alt_calculate_ray(unsigned x, unsigned y) {
    // if (x == 0 and y == 0) {   
    //     Vec3 max_x = Vec3(angles[0], angles[1], angles[2]);
    //     if (max_x.abs() == 0) max_x = Vec3(1, 0, 0);
                
    //     this->angles[1] = asin(max_x.z/max_x.abs());
    //     max_x.z = 0;
    //     this->angles[0] = -asin(max_x.x/max_x.abs());
    //     this->angles[2] = 0;
    //     this->calculate_rotate();
    //     // printf("%f %f %f\n", angles[0], angles[1], angles[2]);
    //     // getchar();
    // }


    Vec3 point(2 * tan(FOV_X / 2) * (x * 1.0 / X - 0.5), 1, 2 * tan(FOV_Y / 2) * (y * 1.0 / Y - 0.5));
    Vec3 screen_point = point + position;
    Vec3 direction(rotate * point);
    return Ray(position, direction.normalized(), x, y, 1);
}

Ray Camera::calculate_ray(unsigned x, unsigned y) {
    if (i_am_fish == 1) {
        double alpha = FOV_X * (x * 1.0 / X - 0.5);
        double beta = FOV_Y * (y * 1.0 / Y - 0.5);
        Vec3 point(cos(beta) * sin(alpha), cos(beta) * cos(alpha), sin(beta));
        Vec3 screen_point = point + position;
        Vec3 direction(rotate * point);
        return Ray(position, direction.normalized(), x, y, 1);
    } else {
        Vec3 point(2 * tan(FOV_X / 2) * (x * 1.0 / X - 0.5), 1, 2 * tan(FOV_Y / 2) * (y * 1.0 / Y - 0.5));
        Vec3 screen_point = point + position;
        Vec3 direction(rotate * point);
        return Ray(position, direction.normalized(), x, y, 1);
    }
}

Camera::Camera(Vec3 position, Vec3 angles, double FOV_X, int X = WIDTH, int Y = HEIGHT, bool i_am_fish):
    position(position), FOV_X(FOV_X), X(X), Y(Y), i_am_fish(i_am_fish) {
    for (short i = 0; i < 3; i++) this->angles[i] = angles[i];
    rotate = calculate_rotate();
    if (i_am_fish) {
        FOV_Y = FOV_X * Y * 1.0 / X;
    } else {
        FOV_Y = 2 * atan(tan(FOV_X / 2) * (Y * 1.0 / X));
    }
}

Camera::Camera() {
    position = Vec3(0, 0, 0);
    angles[0] = 0, angles[1] = 0, angles[2] = 0;
    X = WIDTH;
    Y = HEIGHT;
    FOV_X = 0.7;
    if (i_am_fish) {
        FOV_Y = FOV_X * Y * 1.0 / X;
    } else {
        FOV_Y = 2 * atan(tan(FOV_X / 2) * (Y * 1.0 / X));
    }
    rotate = calculate_rotate();
}

int Camera::height() const{
    return Y;
}

int Camera::width() const{
    return X;
}

//res Ray[Width * Hight]
std::vector<Ray> Camera::create_rays() {
    std::vector<Ray> ret;

    // Vec3 max_x = Vec3(angles[0], angles[1], angles[2]);
    // if (max_x.abs() == 0) max_x = Vec3(0, 1, 0);
                
    //this->angles[1] = asin(max_x.z/max_x.abs());
    // max_x.z = 0;
    //this->angles[0] = -asin(max_x.x/max_x.abs());
    // if (max_x.y < 0) {
    //     if (this->angles[0] < 0) {
    //         this->angles[0] = -3.14 - this->angles[0];
    //     }
    //     else {
    //         this->angles[0] = 3.14 - this->angles[0];
    //     }
    // }
    // this->angles[2] = 0;
    // rotate = calculate_rotate();

    for (unsigned x = 0; x < X; x++) {
        for (unsigned y = 0; y < Y; y++) {
            ret.push_back(calculate_ray(x, y));
        }
    }
    return ret;
}

Object* Camera::clone(std::vector<std::string> const & arg){
   // Vec3 position, Vec3 angles, double distance, double width, double height
   if(arg.size() < 10){
       throw("недостаточно аргументов");
   }
    Camera* rez = new Camera(
        Vec3(atof(arg[0].c_str()), atof(arg[1].c_str()), atof(arg[2].c_str())),
        Vec3(atof(arg[3].c_str()), atof(arg[4].c_str()), atof(arg[5].c_str())),
        atof(arg[6].c_str()), atoi(arg[7].c_str()),  atoi(arg[8].c_str()), atoi(arg[9].c_str())
    );
    return rez;
}

std::pair<int, std::string> Camera::name() const {
    return std::pair<int,std::string>(CAMERA, "Camera");
}

// ------------------------------- scene -------------------------------

Color** Scene::get_res() {
    return res;
}

const Camera* Scene::get_camera() const {
    return camera;
};

Scene::Scene() { }

Scene::~Scene() {
    for (short i = 0; i < camera->width(); i++) {
        delete[] res[i];
    }
    delete[] res;
    delete camera;
}

Scene::Scene(std::vector<Figure*> figures, std::vector<Light_source*> lights, Camera* camera):
    figures(figures), lights(lights), camera(camera) {
}

std::vector<std::string> split(std::string const &s) {
    std::stringstream inp(s);
    std::vector<std::string> ret;
    while (true) {
        std::string tmp;
        inp >> tmp;
        if(!inp) break;
        ret.push_back(tmp);
    }
    return ret;
}

// build figures and lights pos input
void Scene::initialization(const char * input) {
    std::map<std::string, std::pair<int,Object*>> factory;
    std::vector<Object*> zigotes = {
        new Camera(), new Ambient_light(),
        new Point_light(), new Directed_light(),
        new BeautifulPlane(), new BeautifulSphere(),
        new Sphere(), new Tetraedr(), new PolyFigure()
    };


    for (auto obj : zigotes) {
        std::pair<int,std::string> tmp = obj->name();
        factory[tmp.second] = std::pair<int,Object*>(tmp.first, obj);
    }

    std::ifstream inn(input);
    if (!inn) {
        throw("входной файл не открылся");
    }

    std::string s;
    Object * ptr;
    while (getline(inn, s)) {
        auto args = split(s);
        if(args.empty()) continue;
        std::string name = args[0];
        args.erase(args.begin());
        auto it = factory.find(name);
        if (it == factory.end()) continue;
        try{
            ptr = (it->second).second->clone(args);
        } catch(const char* err_st){
            fprintf(stderr, "ошибка при создании объекта с параметрами \"%s\": \n   %s\n", s.c_str(), err_st);
            continue;
        }catch(...){
            fprintf(stderr, "ошибка при создании объекта с параметрами \"%s\": \n %s\n", s.c_str(), strerror(errno));
            continue;
        }
        
        switch ((it->second).first) {
            case CAMERA:
            if(camera) delete camera;
                camera = dynamic_cast<Camera*>(ptr);
                break;
            case LIGHT:
                lights.push_back(dynamic_cast<Light_source*>(ptr));
                break;
            case FIGURE:
                figures.push_back(dynamic_cast<Figure*>(ptr));
                break;
            default:
                break;
        }
    }
}

// returns intersected SmartPoint else valid = false
SmartPoint Scene::get_first_SmartPoint(Ray ray) {
    SmartPoint intersected(false); // точка пересечения
    
    double min = INFINITY; // минимум расстояния до пересеченной фигуры
    for (std::vector<Figure*>::iterator it = figures.begin(); it != figures.end(); it++) {
        SmartPoint point = (*it)->get_intersection_SmartPoint(ray);
        if (point.valid && (point.point - ray.pos).abs() < min) {
            min = (point.point - ray.pos).abs();
            intersected = point;
        }
    }
    //if no intersection returns "negative" result
    return intersected;
}

double Scene::full_intensity_in_point(SmartPoint smartpoint, std::vector<Figure*>* figures_pointer) {
    double ret = 0;
    for(std::vector<Light_source*>::iterator it = lights.begin(); it != lights.end(); it++) {
        ret += (*it)->intensity_in_point(smartpoint, figures_pointer);
    }
    return ret;
}

Ray reflected_ray(SmartPoint smartpoint, Ray incident) {
    Vec3 direction = incident.dir - 2 * smartpoint.normal * dot(smartpoint.normal, incident.dir);
    return Ray(smartpoint.point + smartpoint.normal * ZERO, direction, incident.x, incident.y, incident.intensity * smartpoint.material.t_reflection);
}

Ray refracted_ray(SmartPoint smartpoint, Ray incident) {
    double sin_alpha = vv(smartpoint.normal, incident.dir).abs();
    double cos_alpha = sqrt(1 - sin_alpha * sin_alpha);
    Vec3 point;
    Vec3 direction;
    if (dot(smartpoint.normal, incident.dir) < 0) {
        double sin_beta = sin_alpha / smartpoint.material.n;
        double cos_beta = sqrt(1 - sin_beta * sin_beta);
        direction = ((incident.dir + smartpoint.normal * cos_alpha).normalized() * sin_beta - smartpoint.normal * cos_beta);
        point = smartpoint.point - smartpoint.normal * ZERO;
    } else {
        //return Ray(smartpoint.point + incident.dir * ZERO, incident.dir, incident.x, incident.y, incident.intensity * smartpoint.material.t_refraction);
        double sin_beta = sin_alpha * smartpoint.material.n;
        if (sin_beta > 1) sin_beta = 1;
        double cos_beta = sqrt(1 - sin_beta * sin_beta);
        direction = ((incident.dir - smartpoint.normal * cos_alpha).normalized() * sin_beta + smartpoint.normal * cos_beta);
        point = smartpoint.point + smartpoint.normal * ZERO;
    }
    return Ray(point, direction, incident.x, incident.y, incident.intensity * smartpoint.material.t_refraction);
}

void Scene::trace_ray(Ray ray) {
    if (ray.intensity < STOP_INTENSITY) return;
    SmartPoint intersetion_SmartPoint = get_first_SmartPoint(ray);
    if (intersetion_SmartPoint.valid) {
        Color color = intersetion_SmartPoint.color;
        double light = full_intensity_in_point(intersetion_SmartPoint, &figures); 

        double r = res[ray.x][ray.y].r + ray.intensity * intersetion_SmartPoint.material.t_diffusion * light * color.r;
        double g = res[ray.x][ray.y].g + ray.intensity * intersetion_SmartPoint.material.t_diffusion * light * color.g;
        double b = res[ray.x][ray.y].b + ray.intensity * intersetion_SmartPoint.material.t_diffusion * light * color.b;
        
        // if (r > 255) r = 255;
        // if (g > 255) g = 255;
        // if (b > 255) b = 255;
        res[ray.x][ray.y] = Color(r, g, b);
        
        trace_ray(reflected_ray(intersetion_SmartPoint, ray));
        trace_ray(refracted_ray(intersetion_SmartPoint, ray));
    }
}

void Scene::parallel_trace_ray(Ray * it, int times){
    for( ;times > 0; ++it, --times){
        try{
        trace_ray(*it);
        } catch(const char* err_str){
            fprintf(stderr, "проблемы с обработкой пикселя %d %d:\n     %s\n", it->x, it->y, err_str);
        } catch(...){
            fprintf(stderr, "проблемы с обработкой пикселя:\n     %s\n", strerror(errno));
        }

    }
}

void Scene::render(int thread_count) {
    std::vector<Ray> rays = camera->create_rays();
    int height =  camera->height();
    int width = camera->width();
    res = new Color*[width];
     for (short i = 0; i < width; i++) {
         res[i] = new Color[height];
    }

    int rays_count = rays.size();
    int for_thread = rays_count/thread_count;
    int begin = 0;
    int end = for_thread;

    std::thread ** threads = new std::thread*[thread_count];

    
    for(int i = 0; i < thread_count; ++i){
        try{
        threads[i] = new std::thread(&Scene::parallel_trace_ray, this, &rays[begin], end - begin);
        } catch(...){
            fprintf(stderr, "%d поток не создался. Номер ошибки %s\n", i, strerror(errno));
        }
        begin = end;
        end = for_thread + end > rays_count ? rays_count : for_thread + end;
    }
    
    for(int i = 0; i < thread_count; ++i){
        try{
        threads[i]->join();
        } catch(...){
            fprintf(stderr, "%d проблема с потоком. Номер ошибки %s\n", i, strerror(errno));
        }
    }

    for(int i = 0; i < thread_count; ++i){
        delete threads[i];
    }

    delete[] threads;
    return;
}
