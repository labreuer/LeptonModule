#include "SaveImage.h"
#include <iostream>
#include <ctime>

SaveImage::SaveImage(const QString &label, QWidget *parent, QImage *qimg)
	: QPushButton(label, parent)
{
	this->_qimg = qimg;
}

/*
void SaveImage::save(const QString &fileName, const char* format, int quality)
{
	_qimg->save(fileName, format, quality);
}
*/

void SaveImage::save()
{
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %I.%M.%S", timeinfo);
	QString qs = QString("/home/pi/Pictures/flir/");
	qs.append(buffer);
	qs.append(".png");
	_qimg->save(qs, "png");
}
