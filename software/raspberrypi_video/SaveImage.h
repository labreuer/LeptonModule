#ifndef SAVEIMAGE_H
#define SAVEIMAGE_H

#include <QtCore>
#include <QImage>
#include <QPushButton>

class SaveImage : public QPushButton
{
	Q_OBJECT

public:
	SaveImage(const QString &label, QWidget *parent, QImage *qimg);

public slots:
	// void save(const QString &fileName, const char* format = NULL, int quality = -1);
	void save();

private:
	QImage *_qimg;
};

#endif
