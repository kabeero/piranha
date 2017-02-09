#include <QtGui>
#include <iostream>
#include "playback.h"

using namespace std;

int timeout = 5;

UIplayback::~UIplayback()
{
	abort();
}

void UIplayback::setTimeout(int t)
{
	timeout = t;
}

void UIplayback::run()
{
	loop.reset(new QEventLoop);

	QTimer timer;
	connect(&timer, SIGNAL(timeout()), this, SLOT(capture()));
	timer.start(timeout);
	
	loop->exec();
}

void UIplayback::abort()
{
	if (!loop.isNull())
		loop->quit();
}

void UIplayback::capture()
{
	emit updateNeeded();
}
