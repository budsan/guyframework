#ifndef PARTICLEEDITOR_H
#define PARTICLEEDITOR_H

#include <QMainWindow>
#include <QDockWidget>

namespace Ui {
    class ParticleEditor;
}

class ParticleEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ParticleEditor(QWidget *parent = 0);
    ~ParticleEditor();

private:
    Ui::ParticleEditor *ui;
    QDockWidget *dockProps;
};

#endif // PARTICLEEDITOR_H
