#include "scenewindow.h"

#include "glwindow.h"

#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSignalBlocker>
#include <QSlider>
#include <QVBoxLayout>

namespace {
constexpr int kSliderScale = 10;
constexpr int kSliderMinimum = -100;
constexpr int kSliderMaximum = 100;
constexpr double kSpinMinimum = -10.0;
constexpr double kSpinMaximum = 10.0;
constexpr double kSpinStep = 0.1;

int toSliderValue(double value)
{
    return static_cast<int>(value * kSliderScale);
}

double toSpinValue(int value)
{
    return static_cast<double>(value) / kSliderScale;
}
}

SceneWindow::SceneWindow(QWidget* parent)
    : QWidget(parent)
    , glWindow_(new GLWindow(this))
{
    auto* mainLayout = new QVBoxLayout(this);
    auto* controlLayout = new QHBoxLayout();

    xControls_ = createAxisControls("X", 0.0);
    yControls_ = createAxisControls("Y", 4.5);
    zControls_ = createAxisControls("Z", -3.5);

    controlLayout->addWidget(new QLabel("Light Position", this));
    controlLayout->addSpacing(12);
    controlLayout->addWidget(xControls_.container);
    controlLayout->addWidget(yControls_.container);
    controlLayout->addWidget(zControls_.container);
    controlLayout->addStretch();

    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(glWindow_, 1);

    updateLightPosition();
}

SceneWindow::AxisControls SceneWindow::createAxisControls(const QString& name, double initialValue)
{
    auto* container = new QWidget(this);
    auto* layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    auto* label = new QLabel(name, container);
    auto* slider = new QSlider(Qt::Horizontal, container);
    auto* spinBox = new QDoubleSpinBox(container);

    slider->setRange(kSliderMinimum, kSliderMaximum);
    slider->setValue(toSliderValue(initialValue));
    slider->setFixedWidth(140);

    spinBox->setRange(kSpinMinimum, kSpinMaximum);
    spinBox->setSingleStep(kSpinStep);
    spinBox->setDecimals(1);
    spinBox->setValue(initialValue);
    spinBox->setFixedWidth(72);

    layout->addWidget(label);
    layout->addWidget(slider);
    layout->addWidget(spinBox);

    connect(slider, &QSlider::valueChanged, this, [this, spinBox](int value) {
        {
            const QSignalBlocker blocker(spinBox);
            spinBox->setValue(toSpinValue(value));
        }
        updateLightPosition();
    });

    connect(spinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            [this, slider](double value) {
                {
                    const QSignalBlocker blocker(slider);
                    slider->setValue(toSliderValue(value));
                }
                updateLightPosition();
            });

    return {container, slider, spinBox};
}

void SceneWindow::updateLightPosition()
{
    glWindow_->setLightPosition(static_cast<float>(xControls_.spinBox->value()),
                                static_cast<float>(yControls_.spinBox->value()),
                                static_cast<float>(zControls_.spinBox->value()));
}
