#ifndef SONG_H
#define SONG_H
#include <QObject>
#include <QFileInfo>
#include <id3/tag.h>

class Song : public QObject {
    Q_OBJECT
public:
    Song() : QObject()
    {
        fullFilePath = "";
    }

    Song(QString songPath) : QObject(), fullFilePath(songPath)
    {

    }
    Song(const Song& cSong) : QObject()
    {
        fullFilePath = cSong.getFilePath();
    }
    Song& operator =(const Song& otherSong)
    {
        setFilePath(otherSong.getFilePath());
    }


    QString getFilePath() const { return fullFilePath; }
    void setFilePath(QString newPath) { fullFilePath = newPath; }
    QString getFileName() { return extractFileName(fullFilePath); }
    QString getTitle()
    {
        return extractFileName(fullFilePath);
    }

private:
    QString extractFileName(QString path)
    {
        if(path.isEmpty())
            return path;
        QFileInfo fileInfo(path);
        return fileInfo.fileName();
    }

    QString fullFilePath;
    QString title;
    QString artist;
    QString album;
    int year;
    int durationSeconds;

};

#endif // SONG_H
