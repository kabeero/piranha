#include <QtGui>

class UIplayback : public QObject, public QRunnable
{
	Q_OBJECT

public:
	~UIplayback();
	void run();
	void setTimeout(int);

private slots:
	void abort();
	void capture();

signals:
	void updateNeeded();

private:
	QScopedPointer<QEventLoop> loop;
};
