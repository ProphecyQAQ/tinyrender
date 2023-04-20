#include <vector>
#include <cmath>
#include <iostream>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);
Model *model = nullptr;
const int width  = 800;
const int height = 800;

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    float derror   = std::abs((y1 - y0)/(float)(x1 - x0));
    float error  = 0;
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        if (steep) 
            image.set(y, x, color);
        else 
            image.set(x, y, color);

        error += derror;
        if (error > 0.5) {
            error -= 1;
            y += (y1 > y0)?1:-1;
        }
    }

}

int main(int argc, char **argv) {

    if (2 == argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("../obj/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);

    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec3f v0 = (model->vert(face[0]) + 1.) * 400;
        Vec3f v1 = (model->vert(face[1]) + 1.) * 400;
        Vec3f v2 = (model->vert(face[2]) + 1.) * 400;

        line(v0.x, v0.y, v1.x, v1.y, image, white);
        line(v1.x, v1.y, v2.x, v2.y, image, white);
        line(v2.x, v2.y, v0.x, v0.y, image, white);
    }


    image.flip_vertically();
    image.write_tga_file("output.tga");

    return 0;
}