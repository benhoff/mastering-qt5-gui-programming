#include <iostream>
#include <QImage>
#include <QVector>
#include <QColor>
#include <QFile>
#include <QDir>

#include "virdis_values.h"


int main()
{
    QVector<QColor> colors = get_virdis_values();

    QDir directory = QDir::current();
    bool dir_exists = directory.cd("../../2_section");

    // TODO: some kind of error message
    if (!dir_exists)
        return 1;

    bool pictures_exists = directory.exists("pics");

    if (!pictures_exists)
        directory.mkdir("pics");

    directory.cd("pics");

    foreach(QString filename, directory.entryList())
        directory.remove(filename);

    QString filepath = directory.absolutePath();
    std::cout << "at least a filepath" << std::endl;

    for (int i=0; i < 100; i++)
    {
        int random_number = qrand() % 255;
        QColor color = colors[random_number];

        QImage image(100, 100, QImage::Format_ARGB32);
        image.fill(color);
        QString file(filepath + "/" + QString::number(i) + ".png");
        std::cout << file.toStdString() << std::endl;
        image.save(file);
    }
}

