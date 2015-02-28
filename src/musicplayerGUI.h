#ifndef MUSICPLAYERGUI_H
#define MUSICPLAYERGUI_H
#include <QMainWindow>
#include "musicplayer.h"
#include "progressbar.h"
#include "ticker.h"


class QLabel;
class QProgressBar;
class QPushButton;
class QListWidget;
class QVBoxLayout;
class QSlider;

class MusicPlayerGui : public QMainWindow {

	Q_OBJECT

public:
	MusicPlayerGui(QWidget *parent = 0);

public slots:
	void play();
	void pause();
	void stop();
	void next();
	void previous();
	void setVolume(int value);
	void playlistChanged();
	void addSong();
	void addFolder();
	void removeSelectedSongs();
	void removeAllSongs();
	void removeDuplicates();
	void closeEvent(QCloseEvent *event);
	void playlistDoubleClicked(QListWidgetItem *);
	void updateProgressBar(qint64 time);
	void currentlyPlayedSongChanged();
	void songDurationChanged(qint64 duration);
	void progressBarClicked(qint64 value);
	void shuffleButtonToggled(bool shuffleModeOn);


private:

	void setupGui();
	void loadPlaylist(QString playlistFileName);

	ProgressBar *songProgressBar;
	Ticker *songTitleTicker;

	QPushButton *playButton;
	QPushButton *pauseButton;
	QPushButton *stopButton;
	QPushButton *nextButton;
	QPushButton *previousButton;

	QSlider *volumeSlider;

	QListWidget *playlistView;

	QPushButton *addSongButton;
	QPushButton *delButton;
	QPushButton *sortButton;
	QPushButton *shuffleButton;

	QVBoxLayout *mainLayout;
	QHBoxLayout *bottomLayout;

	MusicPlayer *player;

	const QString playlistFilePath;





};

#endif // MUSICPLAYERGUI_H
