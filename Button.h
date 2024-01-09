#pragma once

#include <string>

#include "surface.h"

namespace Tmpl8
{
    class Button {
    public:
        Button(int x, int y, int width, int height, Pixel color, const std::string& label)
            : x(x), y(y), width(width), height(height), color(color), label(std::move(label)) {}

        void Draw(Surface* surface) {
            surface->Bar(x, y, x + width, y + height, color);
            surface->Print(const_cast<char*>(label.c_str()), x + width / 3, y + height / 2, 0xffffff);
        }

        bool IsInside(int pointX, int pointY) {
            return pointX >= x && pointY >= y && pointX <= (x + width) && pointY <= (y + height);
        }

        void SetColor(Pixel color) {
            this->color = color;
        }

    private:
        int x, y, width, height;
        Pixel color;
        std::string label;
    };
}