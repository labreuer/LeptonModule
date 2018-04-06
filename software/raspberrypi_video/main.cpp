#include <QApplication>
#include <QThread>
#include <QMutex>
#include <QMessageBox>

#include <QColor>
#include <QLabel>
#include <QtDebug>
#include <QString>
#include <QPushButton>
#include <QSlider>

#include "LeptonThread.h"
#include "MyLabel.h"
#include "SaveImage.h"

int main( int argc, char **argv )
{
	const int SliderWidth = 30;
	const int Height = 290;
	const int VideoWidth = 320;
	const int VideoHeight = 240;
	const int BufferWidth = 10;
	const int Width = VideoWidth + 2*BufferWidth + 2*SliderWidth;
	//create the app
	QApplication a( argc, argv );
	
	QWidget *myWidget = new QWidget;
	myWidget->setGeometry(200, 100, Width, Height);

	//create an image placeholder for myLabel
	//fill the top left corner with red, just bcuz
	QImage myImage;
	myImage = QImage(VideoWidth, VideoHeight, QImage::Format_RGB888);
	QRgb red = qRgb(255,0,0);
	for(int i=0;i<80;i++) {
		for(int j=0;j<60;j++) {
			myImage.setPixel(i, j, red);
		}
	}

	//create a label, and set it's image to the placeholder
	MyLabel myLabel(myWidget);
	myLabel.setGeometry(BufferWidth + SliderWidth, BufferWidth, VideoWidth, 240);
	myLabel.setPixmap(QPixmap::fromImage(myImage));

	//create a FFC button
	QPushButton *button1 = new QPushButton("Perform FFC", myWidget);
	button1->setGeometry(VideoWidth/2-50, Height-35, 100, 30);

	//create a thread to gather SPI data
	//when the thread emits updateImage, the label should update its image accordingly
	LeptonThread *thread = new LeptonThread();
	QObject::connect(thread, SIGNAL(updateImage(QImage)), &myLabel, SLOT(setImage(QImage)));

	//connect ffc button to the thread's ffc action
	QObject::connect(button1, SIGNAL(clicked()), thread, SLOT(performFFC()));

	// ghetto save image functionality
	SaveImage *saveImage = new SaveImage("Save Image", myWidget, &thread->myImage);
	saveImage->setGeometry(VideoWidth/2+70, Height-35, 100, 30);
	QObject::connect(saveImage, SIGNAL(clicked()), saveImage, SLOT(save()));

	// min/max labels
	QLabel *minValue = new QLabel(myWidget);
	minValue->setGeometry(SliderWidth, VideoHeight, 40, 40);
	QObject::connect(thread, SIGNAL(updateMinimum(int)), minValue, SLOT(setNum(int)));

	QLabel *maxValue = new QLabel(myWidget);
	maxValue->setGeometry(Width - SliderWidth - 40, VideoHeight, 40, 40);
	QObject::connect(thread, SIGNAL(updateMaximum(int)), maxValue, SLOT(setNum(int)));

	QLabel *realMinValue = new QLabel(myWidget);
	realMinValue->setGeometry(SliderWidth, VideoHeight+40, 40, 40);
	QObject::connect(thread, SIGNAL(updateRealMinimum(int)), realMinValue, SLOT(setNum(int)));

	QLabel *realMaxValue = new QLabel(myWidget);
	realMaxValue->setGeometry(Width - SliderWidth - 40, VideoHeight+40, 40, 40);
	QObject::connect(thread, SIGNAL(updateRealMaximum(int)), realMaxValue, SLOT(setNum(int)));

	// sliders for min/max
	const uint16_t MaxValue = LeptonThread::MaximumPixelValue;
	QSlider *minIntensity = new QSlider(Qt::Vertical, myWidget);
	minIntensity->setGeometry(0, 0, SliderWidth, Height);
	minIntensity->setMaximum(MaxValue);
	QObject::connect(minIntensity, SIGNAL(valueChanged(int)), thread, SLOT(setMinimum(int)));
	if (argc >= 2)
		minIntensity->setSliderPosition(atoi(argv[1]));

	QSlider *maxIntensity = new QSlider(Qt::Vertical, myWidget);
	maxIntensity->setGeometry(Width - SliderWidth, 0, SliderWidth, Height);
	maxIntensity->setMaximum(MaxValue);
	maxIntensity->setSliderPosition(MaxValue);
	QObject::connect(maxIntensity, SIGNAL(valueChanged(int)), thread, SLOT(setMaximum(int)));
	if (argc >= 3)
		maxIntensity->setSliderPosition(atoi(argv[2]));

	thread->start();
	
	myWidget->show();

	return a.exec();
}
