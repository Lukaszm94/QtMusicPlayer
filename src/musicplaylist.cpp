#include "musicplaylist.h"
#include <QVector>
#include <QtAlgorithms>

MusicPlaylist::MusicPlaylist(QObject *parent) : QObject(parent)
{
	playlistList = new QStringList;
	playlistList->clear();
	songsCount = 0;
}

bool MusicPlaylist::addSong(QString fileName)
{
	if(fileName.isEmpty()) {
		return false;
	}

	*playlistList << fileName;
	emit playlistChanged();

	return true;
}

bool MusicPlaylist::addSongs(QStringList fileNames)
{
	if(fileNames.isEmpty()) {
		return false;
	}
	for(QStringList::iterator it = fileNames.begin(); it != fileNames.end(); it++) {
		*playlistList << *it;
	}
	emit playlistChanged();

	return true;
}

bool MusicPlaylist::removeSong(int songNumber)
{
	if(songNumber >= size())
		return false;

	playlistList->removeAt(songNumber);
	emit playlistChanged();
	return true;
}


bool MusicPlaylist::removeSongs(QVector<int> songsNumbers)
{
	if(songsNumbers.empty())
		return true;
	qSort(songsNumbers.begin(), songsNumbers.end(), qGreater<int>());
	foreach(int songNumber ,songsNumbers) {
		this->removeSong(songNumber);
	}
	emit playlistChanged();
	return true;
}

bool MusicPlaylist::removeAllSongs()
{
	if(playlistList->isEmpty())
		return true;
	playlistList->clear();
	qDebug() << "After deleting all, size: "<<playlistList->size();
	emit playlistChanged();
	return true;
}

QString MusicPlaylist::at(int position)
{
	return playlistList->at(position);
}

int MusicPlaylist::size()
{
	return playlistList->size();
}

QStringList MusicPlaylist::toQStringListWithNumbers()
{
	QStringList playlistWithNumbers;
	int i = 1;
	for(QStringList::iterator it = playlistList->begin(); it != playlistList->end();  it++, i++) {
		playlistWithNumbers << QString::number(i) + ". " + QFileInfo(*it).fileName();
	}
	return playlistWithNumbers;
}



