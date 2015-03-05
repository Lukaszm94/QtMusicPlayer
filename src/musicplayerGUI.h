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
    friend class SettingsDialog;

public:
	MusicPlayerGui(QWidget *parent = 0);
    QString getPlaylistLocation();

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
    void openSettingsDialog();


private:
    void loadSettings();
    void saveSettings();
	void setupGui();
	void loadPlaylist(QString playlistFileName);
    void setPlaylistLocation(QString newLocation);

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
    QPushButton *settingsButton;

	QVBoxLayout *mainLayout;
	QHBoxLayout *bottomLayout;

    QString playlistLocation;
	MusicPlayer *player;

	const QString playlistFilePath;

};

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(MusicPlayerGui *parent = 0);

public slots:
    void choosePlaylistLocationButtonPressed();
    void applyButtonClicked();
    void cancelButtonClicked();
    void okButtonClicked();

private:
    void setUserChosenPlaylistFolder(QString folderName);

    QPushButton *choosePlaylistLocationButton;
    QPushButton *applyButton;
    QPushButton *cancelButton;
    QPushButton *okButton;

    QLineEdit *locationLineEdit;

    QString userChosenPlaylistFolder;
    MusicPlayerGui *playerGui;


};

#endif // MUSICPLAYERGUI_H
