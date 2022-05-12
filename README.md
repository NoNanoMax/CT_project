# Проект по компьютерным технологиям, 4 семестр #
## Тема проекта: трассировка лучей ##

Команда:

1. Бабкин Петр, Б05-005
2. Данилов Максим, Б05-005
3. Исмагилов Александр, Б05-002 
4. Латыпов Ильгам

-----
Использовали языки:
1. C++
2. Python

-----
## Краткое описание проекта ## 
Одной из интересных технологий отрисовки трехмерных объектов является трассировка лучей. Суть метода состоит в моделировании оптических систем, в частности, отслеживание поведения лучей света при взаимодействия с поверхностями. В силу моделирования реального поведения света, тени, блики, очертания объектов получаются реалистичными. Среди преимуществ такого метода можно выделить несложное распараллеливания (лучи, участки пространства), возможность отрисовки достаточно сложных объектов. Важно научиться достаточно быстро и эффективно обрабатывать поведение лучей, так как это ключевой фактор в производительности вычислений. Таким образом, работа(как минимум) заключается в изучении оптических свойств материалов, поведения световых лучей, написании эффективных алгоритмов моделирования всего этого, грамотного распараллеливания вычислений и создания библиотеки с понятным и полноценным интерфейсом.  

![](https://github.com/NoNanoMax/CT_project/blob/dev/pictures/ex1.jpg)
![](https://github.com/NoNanoMax/CT_project/blob/dev/pictures/ex2.jpg)
![вставить своего динозавра](https://github.com/NoNanoMax/CT_project/blob/dev/pictures/ex3.jpg)

----

## Описание фигур ##
        BeautifulSphere x, y, z, R, (color), (material)
        BeautifulPlane a, b, c, d, (color), (material)
        Tetraedr x, y, z, (color), (material)
        Tetraedr x, y, z, V1, V2, V3, (color), (material)
        Sphere x, y, z, R, (color), (material), (quality)
        PolyFigure PATH, (color)
## Истчники света ##
        Directed_light x, y, z, intensity
        Point_light x, y, z, intensity
        Ambient_light intensity
## Инициализация камеры ##
        Camera Vec3 pos, Vec3 angles, FOV, Width, Hight, i_am_fish
## C++: реализация библиотеки ##
    main.cpp:
        r.initialization("tmp_info.ass");
        Создание сцены и добавление на нее всех объектов из файла

        r.render();
        Запускает процесс обработки лучей

        Выводит массив цветов экрана для считывания и отрисовки в питоне

    scene.cpp:
        Scene::initialization() реализация
        
        Scene::render(int thread_count = 8)
            camera->create_rays();
            генерирует лучи для заданных настроек камеры

            Создает и запускает потоки по функции Scene::parallel_trace_ray
        
        Scene::parallel_trace_ray:
            trace_ray(Ray x);

        trace_ray(Ray x):
            full_intensity_in_point()
            считает полную освещенность в точки первого пересечения луча

            trace_ray(reflected_ray(intersetion_SmartPoint, ray));
            trace_ray(refracted_ray(intersetion_SmartPoint, ray));
            Рекурсивный запуск отраженного и преломленного лучей из точки пересечения



----
## Python: интерфейс ##

----
## Python(дополнительная часть): построение по серии снимков простых объектов их 3D-модель ##




