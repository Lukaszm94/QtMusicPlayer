#include "musicplayer.h"

//DONE previous song when first is being played (depending on playback mode) RANDOM???
//DONE adding multiple files at once
//DONE random playback mode
//DONE automatic playlist saving and opening
//DONE make shuffle button to work
///		ADDING FILES
//DONE adding multiple folders
//TODO custom folders adding dialog with "include subfolders" checkbox
///		REMOVING FILES
//DONE pop-up menu when remove button pressed
//DONE remove selected files
//DONE remove all files
//TODO remove duplicates

//TODO sorting playlist
//TODO if song description (author, title, etc.) is avaliable- display that data instead of file name
//DONE app crashes when "play" pressed with empty playlist
///     SETTINGS
//DONE add settings icon to main window
//DONE small dialog opens when settings button is pressed
//TODO choosing where to store playlist file

MusicPlayer::MusicPlayer(QObject *parent) : QObject(parent)
{
	player = new QMediaPlayer(parent);
	currentPlaylist = new MusicPlaylist;
	previousSongs = new QList<int>;
	currentSongNumber = 0;
	player->setNotifyInterval(500);
	ignoreStateChangedSignal = false;
	playbackMode = Loop;

	connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChanged(qint64)));
	connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(onDurationChanged(qint64)));
	connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(stateChanged(QMediaPlayer::State)));
	qsrand(1994);
}

void MusicPlayer::play()
{
	if(currentPlaylist->size() == 0)
		return;
	if(player->state() == QMediaPlayer::PlayingState)
		return;
	ignoreStateChangedSignal = true;
	if(player->state() == QMediaPlayer::StoppedState) {
        player->setMedia(QUrl::fromLocalFile(currentPlaylist->at(currentSongNumber)));
	}
	player->play();
	emit songChanged();
	ignoreStateChangedSignal = false;
}

void MusicPlayer::play(int newSongNumber, bool addToPreviousSongsList)
{
	if(currentPlaylist->size() <= newSongNumber || currentPlaylist->size() == 0) {
		this->stop();
		return;
	}
	if(addToPreviousSongsList)
		previousSongs->append(currentSongNumber);

	currentSongNumber = newSongNumber;
	ignoreStateChangedSignal = true;
    player->setMedia(QUrl::fromLocalFile(currentPlaylist->at(currentSongNumber)));
	player->play();
	emit songChanged();
	ignoreStateChangedSignal = false;
	qDebug() << "Currently playing: "<< QString::number(currentSongNumber);
    QString fileName = currentPlaylist->at(currentSongNumber);
}

qint64 MusicPlayer::currentSongDuration()
{
	return player->duration();
}

void MusicPlayer::setPosition(qint64 newPosition)
{
    player->pause();
    QThread::msleep(50);
	player->setPosition(newPosition);
    QThread::msleep(50);
    player->play();
}

int MusicPlayer::getCurrentSongNumber()
{
	return currentSongNumber;
}

bool MusicPlayer::removeSongFromPlaylist(int songNumber)
{
	bool removed = currentPlaylist->removeSong(songNumber);
	if(removed && songNumber < currentSongNumber) {
		currentSongNumber--;
	}

	for(int i = 0; i < previousSongs->size(); i++) {
		int songNumber = previousSongs->at(i);
		if(removed && (songNumber < currentSongNumber)) {
			previousSongs->replace(i, (songNumber - 1 >= 0) ? songNumber - 1: songNumber );
		}
	}
	return removed;
}


void MusicPlayer::pause()
{
	ignoreStateChangedSignal = true;
	player->pause();
	ignoreStateChangedSignal = false;
}

void MusicPlayer::stop()
{
	ignoreStateChangedSignal = true;
	player->stop();
	ignoreStateChangedSignal = false;
}

void MusicPlayer::playNextSong()
{
	if(currentPlaylist->size() == 0)
		return;
	if(currentSongNumber +  1 >= currentPlaylist->size()) {
		if(playbackMode == Sequential) {
			return;
		} else if(playbackMode == Loop) {
			play(0);
			return;
		}
	}

	if(playbackMode == Sequential || playbackMode == Loop) {
		play(currentSongNumber + 1);
	} else if(playbackMode == Random) {
		play(getRandomSongNumber());
	}

}

void MusicPlayer::playPreviousSong()
{
	if(currentPlaylist->size() == 0)
		return;
	if((currentSongNumber - 1) < 0) {
		if(playbackMode == Sequential) {
			return;
		} else if(playbackMode == Loop) {
			play(currentPlaylist->size() - 1, false);
			return;
		}
	}

	if(!previousSongs->isEmpty()) {
		play(previousSongs->last(), false);
		previousSongs->removeLast();
	} else if (playbackMode == Random){
		play(getRandomSongNumber(), false);
	} else if (playbackMode == Loop || playbackMode == Sequential) {
		qDebug() <<"Previous Songs list empty";
		play(currentSongNumber - 1, false);
	}

}



void MusicPlayer::setVolume(int value)
{
	player->setVolume(value);
}

void MusicPlayer::onPlaylistChanged()
{
	emit playlistChanged();
}

MusicPlaylist* MusicPlayer::getPlaylist()
{
	return currentPlaylist;
}

void MusicPlayer::onPositionChanged(qint64 time)
{
	emit updateProgress(time);
}

void MusicPlayer::onDurationChanged(qint64 duration)
{
	emit durationChanged(duration);
}

void MusicPlayer::stateChanged(QMediaPlayer::State state)
{
	if(ignoreStateChangedSignal)
		return;

	if(state == QMediaPlayer::StoppedState) {
		ignoreStateChangedSignal = true;
		if(playbackMode == Sequential && currentSongIsLast())
			return;
		else if(playbackMode == Loop && currentSongIsLast()) {
			play(0);
		}
		else {
			this->playNextSong();
		}
	}
}

void MusicPlayer::setPlaybackMode(MusicPlayer::PlaybackMode newMode)
{
	playbackMode = newMode;
}

bool MusicPlayer::currentSongIsLast()
{
	return ((currentSongNumber+1) >= currentPlaylist->size());
}

int MusicPlayer::getRandomSongNumber()
{
	return (qrand() % currentPlaylist->size());
}
