#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H
#include <QtWidgets>
#include <QtMultimedia>
#include "musicplaylist.h"

class MusicPlayer : public QObject {
	Q_OBJECT

public:
	MusicPlayer(QObject *parent = 0);
	void play(int songNumber, bool addToPreviousSongsList = true); //this function takes care of maintaining previous songs list
	qint64 currentSongDuration();
	void setPosition(qint64 newPosition);
	enum PlaybackMode { Sequential, Loop , Random};
	int getCurrentSongNumber();
	bool removeSongFromPlaylist(int songNumber);
	void setPlaybackMode(MusicPlayer::PlaybackMode newMode);

public slots:
	void play();

	void pause();
	void stop();
	void playNextSong();
	void playPreviousSong();
	void setVolume(int value);
	void onPlaylistChanged();
	MusicPlaylist *getPlaylist();
	void onPositionChanged(qint64 time);
	void onDurationChanged(qint64 duration);
	void stateChanged(QMediaPlayer::State state);


signals:
	void playlistChanged();
	void updateProgress(qint64 time);
	void songChanged();
	void durationChanged(qint64 duration);

private:
	QMediaPlayer *player;
	MusicPlaylist *currentPlaylist;
	QList<int> *previousSongs;
	int currentSongNumber;
	bool ignoreStateChangedSignal;
	PlaybackMode playbackMode;

	bool currentSongIsLast();
	int getRandomSongNumber();

};


#endif // MUSICPLAYER_H
