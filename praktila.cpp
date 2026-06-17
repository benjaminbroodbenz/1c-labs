#define _CRT_SECURE_NO_WARNINGS // Отключаем предупреждения Visual Studio о небезопасных функциях
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Делаем размеры холста константами, чтобы Visual Studio корректно работала с указателем на массив
#define WIDTH 800
#define HEIGHT 800

typedef struct {
    double x, y;
} Point;

typedef struct {
    Point p0, p1, p2, p3;
} Curve;

// Расчет точки кубической кривой Безье на основе полинома Бернштейна
Point calculateBezierPoint(double t, Point p0, Point p1, Point p2, Point p3) {
    double u = 1.0 - t;
    double tt = t * t;
    double uu = u * u;
    double uuu = uu * u;
    double ttt = tt * t;

    Point p;
    p.x = uuu * p0.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x;
    p.y = uuu * p0.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y;
    return p;
}

int main() {
    // Выделяем память с жестко заданными константами WIDTH для совместимости с компилятором MSVC
    unsigned char (*img)[WIDTH][3] = malloc(sizeof(unsigned char[HEIGHT][WIDTH][3]));
    if (!img) {
        return 1;
    }

    // Инициализация фона (Уникальный темный оттенок, например, темно-синий)
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            img[i][j][0] = 15; // R
            img[i][j][1] = 20; // G
            img[i][j][2] = 40; // B
        }
    }

    // Уникальные координаты опорных точек кривой Безье
    Curve myCurve = {
        {80, 720},  // p0 - начальная точка
        {250, 150}, // p1 - первая управляющая
        {550, 680}, // p2 - вторая управляющая
        {720, 180}  // p3 - конечная точка
    };

    int num_steps = 20000;
    for (int i = 0; i <= num_steps; i++) {
        double t = (double)i / num_steps;
        Point p = calculateBezierPoint(t, myCurve.p0, myCurve.p1, myCurve.p2, myCurve.p3);

        int px = (int)p.x;
        int py = (int)p.y;

        // Рендеринг линии с увеличенной толщиной кисти (3х3 пикселя)
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int nx = px + dx;
                int ny = py + dy;
                if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                    // Яркий неоново-бирюзовый цвет кривой
                    img[ny][nx][0] = 0;   // R
                    img[ny][nx][1] = 230; // G
                    img[ny][nx][2] = 180; // B
                }
            }
        }
    }

    // Вывод заголовка графического формата PPM (Текстовый режим P3)
    printf("P3\n%d %d\n255\n", WIDTH, HEIGHT);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%d %d %d ", img[i][j][0], img[i][j][1], img[i][j][2]);
        }
        printf("\n");
    }

    free(img);
    return 0;
}