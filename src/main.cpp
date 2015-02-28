#include <QApplication>
#include <QMainWindow>

#include "musicplayer.h"
#include "musicplaylist.h"
#include "musicplayerGUI.h"



int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	MusicPlayerGui *gui = new MusicPlayerGui();
	gui->show();
	return app.exec();
}
