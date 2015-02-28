#ifndef MUSICPLAYLIST_H
#define MUSICPLAYLIST_H

#include <QtMultimedia>

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
	QString at(int position); //returns full file path
	int size();



private:
	QStringList *playlistList;

	int songsCount;

signals:
	void playlistChanged();


};


#endif // MUSICPLAYLIST_H
