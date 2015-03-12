#ifndef MUSICPLAYLIST_H
#define MUSICPLAYLIST_H

#include <QtMultimedia>
#include "song.h"

class MusicPlaylist : public QObject {
	Q_OBJECT

public:
	MusicPlaylist(QObject *parent = 0);
	bool addSong(QString fileName);
	bool addSongs(QStringList fileNames);
	bool removeSong(int songNumber);
	bool removeSongs(QVector<int>);
	bool removeAllSongs();
	QStringList toQStringListWithNumbers();
    Song at(int position); //returns full file path
	int size();



private:
    QList<Song> *playlistList;

	int songsCount;

signals:
	void playlistChanged();


};


#endif // MUSICPLAYLIST_H
