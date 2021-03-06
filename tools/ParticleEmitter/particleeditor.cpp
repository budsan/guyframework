#include "particleeditor.h"
#include "ui_particleeditor.h"

#include "particleparameters.h"

ParticleEditor::ParticleEditor(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::ParticleEditor)
{
	ui->setupUi(this);

    connect(ui->actionNew    , SIGNAL(triggered()), ui->widget, SLOT(newEmitter()));
    connect(ui->actionOpen   , SIGNAL(triggered()), ui->widget, SLOT(open()));
    connect(ui->actionSave   , SIGNAL(triggered()), ui->widget, SLOT(save()));
    connect(ui->actionSave_as, SIGNAL(triggered()), ui->widget, SLOT(saveAs()));

	dockProps = new QDockWidget(tr("Parameters Editor"), this);
	dockProps->setObjectName("parameters");
	dockProps->setWidget(ui->widget->getParticleParameters());
	addDockWidget(Qt::RightDockWidgetArea, dockProps);
}

ParticleEditor::~ParticleEditor()
{

	delete ui;
}
