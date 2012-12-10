#ifndef PARTICLEPARAMETERS_H
#define PARTICLEPARAMETERS_H

#include <QWidget>
#include <QColor>

#include "tools/particleemitter.h"
#include "graphics/color.h"

namespace Ui {
	class ParticleParameters;
}

class QPushButton;
class ParticleParameters : public QWidget
{
	Q_OBJECT

public:
	explicit ParticleParameters(QWidget *parent = 0);
	~ParticleParameters();

	void newEmitter();
	ParticleEmitter &getEmitter() { return m_parameters; }

signals:
	void EmitterChanged();

private:
	Ui::ParticleParameters *ui;

	ParticleEmitter m_parameters;
	bool m_applyAutomatically;

	QColor StartColor;
	QColor EndColor;
	void setBackgroundColor(QPushButton* but, const QColor &col);
	static rgba   toRGBA  (const QColor &col);
	static QColor toQColor(const rgba   &col);
public slots:
	void UpdateUI();
	void ApplyChanges();

private slots:
	void on_ApplyBut_clicked();
	void on_EndColorBut_clicked();
	void on_StartColorBut_clicked();
};

#endif // PARTICLEPARAMETERS_H
