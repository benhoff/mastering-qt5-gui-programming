#ifndef ROTATETRANSFORM_H
#define ROTATETRANSFORM_H

#include <Qt3DCore/QNode>
#include <Qt3DCore/QTransform>



/* Concrete class to expose a `user_angle` property
 * we'll use a `QPropertyAnimation` to vary the `user_angle`
 * from "0" to "360" over 10 seconds.
 *  Check out `_rotate` slot for implementation
 */
class RotateTransform : public Qt3DCore::QTransform
{
    Q_OBJECT
    Q_PROPERTY(float user_angle READ get_user_angle WRITE set_user_angle NOTIFY notify_angle_changed)

public:
    explicit RotateTransform()
    {
        QObject::connect(this, &RotateTransform::notify_angle_changed, this, &RotateTransform::_rotate);
    }

    ~RotateTransform()
    {
    }

    float get_user_angle()
    {
        return _user_angle;
    }

    void set_user_angle(float new_angle)
    {
        _user_angle = new_angle;
        emit notify_angle_changed();
    }

signals:
    void notify_angle_changed();
private slots:
    void _rotate()
    {
       // create a 4x4 matrix
       QMatrix4x4 matrix;
       // rotate based on angle
       matrix.rotate(_user_angle, QVector3D(0, 1, 0));
       matrix.translate(QVector3D(20, 0, 0));
       // set the matrix to our new matrix!
       setMatrix(matrix);
    }

private:
    float _user_angle;
};
#endif // ROTATETRANSFORM_H
