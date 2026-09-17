#include "mainwindow.h"
#include <singleapplication.h>
#include <QTranslator>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QDir>

#include <QStandardPaths>

static QString timePoint;

//日志生成
void LogMsgOutput(QtMsgType type,
	const QMessageLogContext&,
	const QString& msg)
{
	static QMutex mutex; //日志代码互斥锁

	// 持有锁
	mutex.lock();

	// Critical Resource of Code
	QByteArray localMsg = msg.toLocal8Bit();
	QString log;

	switch (type) {
	case QtDebugMsg:
		log.append(QString("Debug %1")
			.arg(localMsg.constData()));
		break;
	case QtInfoMsg:
		log.append(QString("Info: %1")
			.arg(localMsg.constData()));
		break;
	case QtWarningMsg:
		log.append(QString("Warning: %1")
			.arg(localMsg.constData()));
		break;
	case QtCriticalMsg:
		log.append(QString("Critical: %1")
			.arg(localMsg.constData()));
		break;
	case QtFatalMsg:
		log.append(QString("Fatal: %1")
			.arg(localMsg.constData()));
		break;
	}

	QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	if (dataDir.isEmpty()) {
		dataDir = QDir::homePath() + "/.local/share/DrCOM_JLU_Qt";
	}
	QDir dir(dataDir);
	dir.mkpath("logs");
	QFile file(dir.filePath(QString("logs/log%1.lgt").arg(timePoint)));
	if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
		QTextStream out(&file);
		out << log << Qt::endl;
		file.close();
	}

	// 释放锁
	mutex.unlock();
}

int main(int argc, char *argv[])
{

	Q_INIT_RESOURCE(DrCOM_JLU_Qt);
	SingleApplication a(argc, argv);
	a.setApplicationName("DrCOM_JLU_Qt");
	a.setOrganizationName("DrCOM_JLU_Qt");

	//release模式输出日志到文件
#ifndef QT_DEBUG
	timePoint = QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
	qInstallMessageHandler(LogMsgOutput);
#endif

	SingleApplication::setQuitOnLastWindowClosed(false);

	qDebug() << "...main...";

	QFont font = a.font();
	font.setPointSize(10);
	font.setFamily("Microsoft YaHei");
	a.setFont(font);

	QTranslator translator;
    if (!translator.load(":/ts/DrCOM_zh_CN.qm")) {
        qWarning() << "Failed to load translation file!";
    }
	a.installTranslator(&translator);

    MainWindow w(&a);
	QObject::connect(&a, &SingleApplication::instanceStarted, [&w]() {
		qDebug() << "One instance had started. Showing login window.";
		w.ShowLoginWindow();
	});

    QSettings s(SETTINGS_FILE_NAME);
    bool bHideWindow = s.value(ID_HIDE_WINDOW, false).toBool();
	int restartTimes = s.value(ID_RESTART_TIMES, 0).toInt();
	bool minimizedArg = a.arguments().contains("--minimized") || a.arguments().contains("-m") || a.arguments().contains("--tray");
	qDebug() << "main: restartTimes=" << restartTimes << "bHideWindow=" << bHideWindow << "minimizedArg=" << minimizedArg;

    if (minimizedArg || bHideWindow) {
        qDebug() << "not show window caused by user settings or --minimized";
    } else if (restartTimes > 0) {
        // 是自行重启不显示窗口
        qDebug() << "not show window caused by self restart";
    } else {
		qDebug() << "show caused by normal start";
		w.show();
	}
	return a.exec();
}
