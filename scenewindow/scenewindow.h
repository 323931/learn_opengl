#pragma once

#include <QString>
#include <QWidget>

class GLWindow;
class QDoubleSpinBox;
class QSlider;

class SceneWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SceneWindow(QWidget* parent = nullptr);

private:
    struct AxisControls {
        QWidget* container = nullptr;
        QSlider* slider = nullptr;
        QDoubleSpinBox* spinBox = nullptr;
    };

    AxisControls createAxisControls(const QString& name, double initialValue);
    void updateLightPosition();

    GLWindow* glWindow_ = nullptr;
    AxisControls xControls_;
    AxisControls yControls_;
    AxisControls zControls_;
};
