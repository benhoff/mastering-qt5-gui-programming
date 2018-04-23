#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <QtGui/QOpenGLFunctions>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QAbstractTextureImage>
#include <Qt3DRender/QTextureImageDataPtr>
#include <Qt3DRender/QTextureImageData>
#include <Qt3DRender/QTextureImageDataGenerator>

// HELP: https://github.com/andeplane/LANVis/blob/master/libs/SimVis/src/utils/marchingcubestabletexture.cpp

typedef Qt3DRender::QTextureImageDataGenerator QTextureImageDataGenerator;

class TextureGenerator : public QTextureImageDataGenerator {
public:
    // TODO: add reference to abstract video surface. probably.
    TextureGenerator (int height, int width, int layers, int block_size)
        : QTextureImageDataGenerator()
        , _height(height)
        , _width(width)
        , _layers(layers)
        , _block_size(block_size)
    {
    }

    ~TextureGenerator() {}

    typedef Qt3DRender::QTextureImageDataPtr QTextureImageDataPtr;

    QTextureImageDataPtr operator()() override
    {
        QTextureImageDataPtr data = QTextureImageDataPtr::create();
        QByteArray image_data;

        data.data()->setWidth(_width);
        data.data()->setHeight(_height);
        data.data()->setLayers(_layers);
        data.data()->setDepth(1);
        data.data()->setFaces(1);
        data.data()->setLayers(1);
        data.data()->setMipLevels(1);

        data.data()->setData(image_data, _block_size);

        data.data()->setFormat();
        data.data()->setPixelFormat();
        data.data()->setPixelType();
        data.data()->setTarget();
        return data;
    }
    bool operator ==(const QTextureImageDataGenerator &other) const override;
    QT3D_FUNCTOR(TextureGenerator)

private:
    int _height;
    int _width;
    int _layers;
    int _block_size;
    QOpenGLTexture::PixelFormat _pixel_format;
    QOpenGLTexture::PixelType _pixel_type;
};

class Texture2D
{
public:
    Texture2D();
};

class Texture2D : public QAbstractTexture {
public:
    explict Texture2D(QNode *parent = nullptr)
        : QAbstractTexture(QAbstractTexture::Target2D, parent)
        , _block_size(-1)
    {
    }

    typedef Qt3DRender::QAbstractTexture QAbstractTexture;
    typedef Qt3DRender::QTextureImageDataGeneratorPtr QTextureImageDataGeneratorPtr;


    QTextureImageDataGeneratorPtr dataGenerator() const
    {
        // FIXME
        new TextureGenerator(height(), width(), _layers, _block_size);
        // return QTextureImageDataGeneratorPtr(new TextureGenerator());
    }

    void setWidth(int width) override
    {
        QAbstractTexture::setWidth(width);
    }

    void setHeight(int height) override
    {
        QAbstractTexture::setHeight(height);
    }

    void set_block_size(int block_size)
    {
        _block_size = block_size;
    }

    void set_layers(int layers)
    {
        _layers = layers;
    }

    void set_pixel_format(QOpenGLTexture::PixelFormat pixelFormat)
    {
        _pixel_format = pixelFormat;
    }

    void set_pixel_type(QOpenGLTexture::PixelType pixelType)
    {
        _pixel_type = pixelType;
    }

private:
    int _block_size;
    int _layers;
    QOpenGLTexture::PixelFormat _pixel_format;
    QOpenGLTexture::PixelType _pixel_type;
};

#endif // TEXTURE2D_H
