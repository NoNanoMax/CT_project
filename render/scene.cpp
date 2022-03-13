/*

    scene.cpp

Программный файл для работы сцены

*/

#include <vector>


#include "math.h"
#include "scene.h"



Object::Object(){

}

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

Ray Camera::calculate_ray(unsigned x, unsigned y) {

    Vec3 point(2*tan(FOV_X/2) * (x*1.0 / X - 0.5), 1, 2*tan(FOV_Y/2) * (y *1.0/ Y - 0.5));
    Vec3 screen_point = point + position;
    Vec3 direction(rotate *point);
    return Ray(position, direction.normalized());
}

Camera::Camera(Vec3 position, Vec3 angles, double FOV_X, int X = WIDTH, int Y = HEIGHT):
    position(position), FOV_X(FOV_X), X(X), Y(Y) {
    for (short i = 0; i < 3; i++) this->angles[i] = angles[i];
    rotate = calculate_rotate();
    FOV_Y = 2* atan(tan(FOV_X/2)*(Y*1.00/X));
}

Camera::Camera() {
    position = Vec3(0, 0, 0);
    angles[0] = 0, angles[1] = 0, angles[2] = 0;
    X = WIDTH;
    Y = HEIGHT;
    FOV_X = 0.7;
    FOV_Y = 2* atan(tan(0.7/2)* (Y/X));

    rotate = calculate_rotate();
}

int Camera::height() const{
    return Y;
}

int Camera::width() const{
    return X;
}

//res Ray[Width][Hight]
std::vector<std::vector<Ray>> Camera::create_rays() {
    std::vector<std::vector<Ray>> ret;
    std::vector<Ray> _a;
    for (int i = 0; i < X; i++)
        ret.push_back(_a);
    for (unsigned x = 0; x < X; x++) {
        for (unsigned y = 0; y < Y; y++) {
            ret[x].push_back(calculate_ray(x, y));
        }
    }
    return ret;
}


Object* Camera::clone(std::vector<std::string> const & arg){
   // Vec3 position, Vec3 angles, double distance, double width, double height
    assert(arg.size() >= 9);
    Camera* rez = new Camera(
        Vec3(atof(arg[0].c_str()), atof(arg[1].c_str()), atof(arg[2].c_str())),
        Vec3(atof(arg[3].c_str()), atof(arg[4].c_str()), atof(arg[5].c_str())),
        atof(arg[6].c_str()), atof(arg[7].c_str()),  atof(arg[8].c_str())
    );
    /*
    rez->position = ;
    rez->angles[0] = ;
    rez->angles[1] = ;
    rez->angles[2] = 
    rez->FOV_X = ;
    rez->X = a;
    rez->Y = atof(arg[8].c_str());
    rez->rotate = calculate_rotate();*/
    return rez;
}


std::pair<int,std::string> Camera::name() const{
    return std::pair<int,std::string>(CAMERA, "Camera");
}


// ------------------------------- scene -------------------------------

Color** Scene::get_res() {
    return res;
}

const Camera* Scene::get_camera() const{
    return camera;
};

Scene::Scene() {
    // res = new Color*[WIDTH];
    // for (short i = 0; i < WIDTH; i++) {
    //     res[i] = new Color[HEIGHT];
    // }
}

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


std::vector<std::string> split(std::string const &s){
    std::stringstream inp(s);
    std::vector<std::string> ret;
    while(true){
        std::string tmp;
        inp >> tmp;
        if(!inp) break;
        ret.push_back(tmp);
    }
    return ret;
}

//build figures and lights from input, deafult "input.txt"
void Scene::initialization(const char * input) {
    std::map<std::string, std::pair<int,Object*>> factory;
    std::vector<Object*> zigotes ={
        new Camera(),
        new Ambient_light(), new Point_light(), new Directed_light(),
        new BeautifulPlane(), new BeautifulSphere()
    };

    for(auto obj : zigotes){
        std::pair<int,std::string> tmp = obj->name();
        factory[tmp.second] = std::pair<int,Object*>(tmp.first, obj);
    }

    std::ifstream inn(input);
    if(!inn){
        perror("no input file:");
    }
    std::string s;
    while(getline(inn, s)){
        auto args = split(s);
        assert(!args.empty());
        std::string name = args[0];
        args.erase(args.begin());
        auto it = factory.find(name);
        if(it == factory.end()) continue;
        Object * ptr = (it->second).second->clone(args);
        switch ((it->second).first){
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


//returns normal at intesection point else vector direction equals (2, 0, 0)
inline Ray Scene::get_first_intersection(Ray ray) {
    Ray intersected(Vec3(0,0,0), Vec3(2,0,0)); // точка пересечения

    if (ray.dir.x == 0 and ray.dir.y == 0 and ray.dir.z == 0)
        return intersected; //костыль для ambient light

    double min = 1e5; // минимум расстояния до пересеченной фигуры
    for (std::vector<Figure*>::iterator it = figures.begin(); it != figures.end(); it++) {
        Ray point = (*it)->does_intersect(ray); 
        if (!(point.dir.x == 2) && (point.from - ray.from).abs() < min) {
            min = (point.from - ray.from).abs();
            intersected = point;
        }
    }
    //if no intersection returns "negative" result
    return Ray(intersected.from, intersected.dir);
}

double Scene::full_intensity_in_point(Vec3 point, Vec3 normal) {
    double ret = 0;
    for(std::vector<Light_source*>::iterator it = lights.begin(); it != lights.end(); it++) {
        Ray target_ray = Ray(point, ((*it)->get_position(point) - point).normalized());
        Ray res = get_first_intersection(target_ray);
        if (res.dir.x == 2)
            ret += (*it)->intensity_in_point(point, normal);
    }
    return ret;
}


Color Scene::trace_ray(Ray ray) {

    Color ret;
    ret.r = 30; ret.b = 255; ret.g = 144; //set background color

    Ray intersetion_point = get_first_intersection(ray);
    if (intersetion_point.dir.x != 2) {
        // ret.make("black");
        
        
        double light = full_intensity_in_point(intersetion_point.from, intersetion_point.dir) * 255;
        // printf("%lf %lf %lf\n", ray.from.x, ray.from.y, ray.from.z);

        ret.r = short(light)*1;//abs(dot(intersetion_point.dir, Vec3(1,0,0)));
        ret.g = short(light)*1;//abs(dot(intersetion_point.dir, Vec3(0,1,0)));
        ret.b = short(light)*1;//abs(dot(intersetion_point.dir, Vec3(0,0,1)));

        if (ret.r > 255) ret.r = 255;
        if (ret.g > 255) ret.g = 255;
        if (ret.b > 255) ret.b = 255;
    }
    return ret;
}

void Scene::render() {
    std::vector<std::vector<Ray>> rays = camera->create_rays();

    int height =  camera->height();
    int width = camera->width();

    res = new Color*[width];
     for (short i = 0; i < width; i++) {
         res[i] = new Color[height];
    }

    for (unsigned x = 0; x < width; x++) {
        for (unsigned y = 0; y < height; y++) {
            // int pos_x = rays[x][y].x, pos_y = rays[x][y].y;
            res[x][y] = trace_ray(rays[x][y]);
            
        }
    }
}



