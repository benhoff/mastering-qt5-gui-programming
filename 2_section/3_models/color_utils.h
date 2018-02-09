#ifndef COLOR_UTILS_H
#define COLOR_UTILS_H

#include <QColor>

inline QColor get_color(float r, float g, float b)
{
    int red = (int)((r)*256.0);
    int green = (int)((g)*256.0);
    int blue = (int)((b)*256.0);

    return QColor(red, green, blue);
}

#endif // COLOR_UTILS_H
