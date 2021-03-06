#include "musicplayerGUI.h"
#include "settingsdialog.h"
#include <QtWidgets>
#include <QVBoxLayout>
#include <chrono>
#include "id3/tag.h"
#include <id3.h>
#include <id3/id3lib_frame.h>
#include <id3/misc_support.h>


MusicPlayerGui::MusicPlayerGui(QWidget *parent) :QMainWindow(parent), playlistFileName("playlist.pla")
{
	player = new MusicPlayer;
	setupGui();
    loadSettings();
    loadPlaylist(playlistLocation + "/" + playlistFileName);

    ID3_Tag myTag;
    myTag.Link("abc.mp3");
    qDebug()<<ID3_GetArtist(&myTag);
    /*ID3_Tag::Iterator* iter = myTag.CreateIterator();
      ID3_Frame* myFrame = NULL;
      while (NULL != (myFrame = iter->GetNext()))
      {
        qDebug()<<myFrame->GetDescription()<<": "<<myFrame->GetField(ID3FN_TEXT)->GetRawText();
      }*/
}

QString MusicPlayerGui::getPlaylistLocation()
{
    return playlistLocation;
}

void MusicPlayerGui::play()
{
	int selectedItemsNumber = playlistView->selectedItems().count();
	int currentRow = playlistView->currentRow() - selectedItemsNumber + 1;
	if( selectedItemsNumber > 1) {
		playlistView->clearSelection();
		playlistView->setCurrentRow(currentRow);
	}
	if(player->getCurrentSongNumber() != currentRow && currentRow > -1) {
		player->play(currentRow);
		return;
	}
	player->play();
}

void MusicPlayerGui::pause()
{
	player->pause();
}

void MusicPlayerGui::stop()
{
	songTitleTicker->setText("");
	player->stop();
}

void MusicPlayerGui::next()
{
	player->playNextSong();
}

void MusicPlayerGui::previous()
{
	player->playPreviousSong();
}

void MusicPlayerGui::setVolume(int value)
{
	player->setVolume(value);
}

void MusicPlayerGui::playlistChanged()
{
	playlistView->clear();
	QStringList songsList = player->getPlaylist()->toQStringListWithNumbers();

	playlistView->addItems(songsList);
}

void MusicPlayerGui::addSong()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Add songs"), "G:/Muzyka", tr("Music (*.mp3)"));
	player->getPlaylist()->addSongs(fileNames);
    foreach(QString name , fileNames)
    {
        ID3_Tag myTag;
        myTag.Link(name.toStdString().c_str());
        qDebug() <<"Song: "<< name;
        ID3_Frame* myFrame;
        if ( myFrame = myTag.Find ( ID3FID_TITLE ) )
        {
            char title[ 1024 ];

            myFrame->Field ( ID3FN_TEXT ).Get ( title, 1024 );

            qDebug() << "Title: " << title;
        } else {
            qDebug() << "Unable to find tag ALBUM";
        }
    }

}

void MusicPlayerGui::addFolder()
{
	QFileDialog foldersDialog;
	foldersDialog.setFileMode(QFileDialog::DirectoryOnly);
	foldersDialog.setOption(QFileDialog::DontUseNativeDialog, true);
	QListView *l = foldersDialog.findChild<QListView*>("listView");
	if (l) {
		l->setSelectionMode(QAbstractItemView::MultiSelection);
	}
	QTreeView *t = foldersDialog.findChild<QTreeView*>();
	if (t) {
		t->setSelectionMode(QAbstractItemView::MultiSelection);
	}
	QStringList folderNames;
	if(foldersDialog.exec()) {
		folderNames = foldersDialog.selectedFiles();
		QStringList nameFilters;
		nameFilters << "*.mp3";
		foreach(QString folderName, folderNames) {
			qDebug() << "Adding files from folder: "<<folderName;
			QDir currentFolder(folderName);
			currentFolder.setNameFilters(nameFilters);
			QStringList filesInCurrentFolder = currentFolder.entryList(QDir::Files);
			foreach(QString fileName , filesInCurrentFolder) {
				player->getPlaylist()->addSong(folderName + "/" + fileName);
			}
		}
	}

}

void MusicPlayerGui::removeSelectedSongs()
{
	QList<QListWidgetItem *> selectedItems = playlistView->selectedItems();
	QVector<int> songsNumbers(selectedItems.count() + 5);
	songsNumbers.clear();
	foreach(QListWidgetItem *item , selectedItems) {
		songsNumbers.push_back(playlistView->row(item));
	}
	player->getPlaylist()->removeSongs(songsNumbers);

}

void MusicPlayerGui::removeAllSongs()
{
	player->getPlaylist()->removeAllSongs();
}

void MusicPlayerGui::removeDuplicates()
{
	qDebug() << "Removing duplicates...";
}

void MusicPlayerGui::closeEvent(QCloseEvent *event)
{
    savePlaylist();
    saveSettings();
	event->accept();
}

void MusicPlayerGui::playlistDoubleClicked(QListWidgetItem *)
{
	int selectedSong = playlistView->currentRow();
	player->play(selectedSong);
}

void MusicPlayerGui::updateProgressBar(qint64 time)
{
	songProgressBar->setValue(time);
}

void MusicPlayerGui::currentlyPlayedSongChanged()
{
	playlistView->setCurrentRow(player->getCurrentSongNumber());
    QString fileName = QFileInfo(player->getPlaylist()->at(player->getCurrentSongNumber()).getFilePath()).fileName();
	songTitleTicker->setText( fileName + "  +++  ");
}

void MusicPlayerGui::songDurationChanged(qint64 duration)
{
	songProgressBar->setMaximum(duration);
}

void MusicPlayerGui::progressBarClicked(qint64 value)
{
	player->setPosition(value);
}

void MusicPlayerGui::shuffleButtonToggled(bool shuffleModeOn)
{
    MusicPlayer::PlaybackMode mode;
    if(shuffleModeOn)
        mode = MusicPlayer::Random;
    else
        mode = MusicPlayer::Loop;
    setPlayerPlaybackMode(mode);
}

void MusicPlayerGui::setPlayerPlaybackMode(MusicPlayer::PlaybackMode newMode)
{
   player->setPlaybackMode(newMode);
   if(newMode == MusicPlayer::Random && !shuffleButton->isChecked()) {
       shuffleButton->setChecked(true);
       qDebug() << "Setting shuffle button to checked";
   }
}

void MusicPlayerGui::openSettingsDialog()
{
    QDialog *settingsDialog = new SettingsDialog(this);

    settingsDialog->show();
}


void MusicPlayerGui::loadSettings()
{
    QSettings settings("Meyer Technologies", "Simplicity");
    settings.beginGroup("player");
    setPlaylistLocation(settings.value("playlistLocation").toString());
    int playbackMode = settings.value("playbackMode").toInt();
    qDebug() << "Playback mode read from settings: "<<playbackMode;
    this->setPlayerPlaybackMode(static_cast<MusicPlayer::PlaybackMode> (playbackMode));
    settings.endGroup();
    qDebug() << "Playlist locations read from settings: "<<playlistLocation;
}

void MusicPlayerGui::saveSettings()
{
    QSettings settings("Meyer Technologies", "Simplicity");
    settings.beginGroup("player");
    settings.setValue("playlistLocation", playlistLocation);

    MusicPlayer::PlaybackMode playbackMode;
    if(shuffleButton->isChecked())
        playbackMode = MusicPlayer::Random;
    else
        playbackMode = MusicPlayer::Loop;
    qDebug() <<"Saving settings, playback mode value: "<<playbackMode;

    settings.setValue("playbackMode", QString::number(playbackMode));
    settings.endGroup();
}

void MusicPlayerGui::savePlaylist()
{
    QFile saveFile(playlistLocation + "/" + playlistFileName);
    if(!saveFile.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, tr("Music Player"), tr("Cannot write file %1:\n%2.").arg(saveFile.fileName()).arg(saveFile.errorString()));
            return;
    }
    QDataStream out(&saveFile);
    out.setVersion(QDataStream::Qt_5_3);

    for(int i = 0; i< player->getPlaylist()->size(); i++) {
        out << player->getPlaylist()->at(i).getFilePath();
    }
    saveFile.close();
}

void MusicPlayerGui::setupGui()
{
	songTitleTicker = new Ticker;
	songProgressBar = new ProgressBar;
	songProgressBar->setTextVisible(false);

	volumeSlider = new QSlider;
	volumeSlider->setOrientation(Qt::Horizontal);
	volumeSlider->setMinimumWidth(volumeSlider->sizeHint().width());
	connect(volumeSlider, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));
	volumeSlider->setValue(70);

	playButton = new QPushButton;
    playButton->setIcon(QIcon(":/icons/icons/play.png"));
	connect(playButton, SIGNAL(clicked()), this, SLOT(play()));

	pauseButton = new QPushButton;
    pauseButton->setIcon(QIcon(":/icons/icons/pause.png"));
	connect(pauseButton, SIGNAL(clicked()), this, SLOT(pause()));

	stopButton = new QPushButton;
    stopButton->setIcon(QIcon(":/icons/icons/stop.png"));
	connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));

	nextButton = new QPushButton;
    nextButton->setIcon(QIcon(":/icons/icons/next.png"));
	connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));

	previousButton = new QPushButton;
    previousButton->setIcon(QIcon(":/icons/icons/previous.png"));
	connect(previousButton, SIGNAL(clicked()), this, SLOT(previous()));

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(playButton);
	buttonsLayout->addWidget(pauseButton);
	buttonsLayout->addWidget(stopButton);
	buttonsLayout->addWidget(previousButton);
	buttonsLayout->addWidget(nextButton);
	buttonsLayout->addWidget(volumeSlider);

	playlistView = new QListWidget;
	playlistView->setSelectionMode(QAbstractItemView::ExtendedSelection);

	addSongButton = new QPushButton;
    addSongButton->setIcon(QIcon(":/icons/icons/add.png"));

	sortButton = new QPushButton;
    sortButton->setIcon(QIcon(":/icons/icons/sort.png"));

	QMenu *sortMenu = new QMenu(this);
	QAction *sortByTitleAction = new QAction(tr("Sort by title"), this);
	sortMenu->addAction(sortByTitleAction);
	sortButton->setMenu(sortMenu);

	QMenu *addMenu = new QMenu(this);
	QAction *addSongAction = new QAction(tr("Songs"), this);
    addSongAction->setIcon(QIcon(":/icons/icons/addfile.png"));
	QAction *addFolderAction = new QAction(tr("Folders"), this);
    addFolderAction->setIcon(QIcon(":/icons/icons/addfolder.png"));
	addMenu->addAction(addSongAction);
	addMenu->addAction(addFolderAction);
	addSongButton->setMenu(addMenu);

	connect(addSongAction, SIGNAL(triggered()), this, SLOT(addSong()));
	connect(addFolderAction, SIGNAL(triggered()), this, SLOT(addFolder()));

	delButton = new QPushButton;
    delButton->setIcon(QIcon(":/icons/icons/del.png"));

	QMenu *delMenu = new QMenu(this);
	QAction *removeSelectedAction = new QAction(tr("Remove selected"), this);
	connect(removeSelectedAction, SIGNAL(triggered()), this, SLOT(removeSelectedSongs()));
	QAction *removeAllAction = new QAction(tr("Remove all"), this);
	connect(removeAllAction, SIGNAL(triggered()), this, SLOT(removeAllSongs()));
	QAction *removeDuplicatesAction = new QAction(tr("Remove duplicates"), this);
	connect(removeDuplicatesAction, SIGNAL(triggered()), this, SLOT(removeDuplicates()));

	delMenu->addAction(removeSelectedAction);
	delMenu->addAction(removeAllAction);
	delMenu->addAction(removeDuplicatesAction);
	delButton->setMenu(delMenu);

	shuffleButton = new QPushButton;
    shuffleButton->setIcon(QIcon(":/icons/icons/shuffle.png"));
	shuffleButton->setCheckable(true);

    settingsButton = new QPushButton;
    settingsButton->setIcon(QIcon(":/icons/icons/settings.png"));


	bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(addSongButton);
	bottomLayout->addWidget(delButton);
	bottomLayout->addWidget(sortButton);
	bottomLayout->addStretch();
    bottomLayout->addWidget(shuffleButton);
    bottomLayout->addWidget(settingsButton);

	mainLayout = new QVBoxLayout;
	mainLayout->addWidget(songTitleTicker);
	mainLayout->addWidget(songProgressBar);
	mainLayout->addLayout(buttonsLayout);
	mainLayout->addWidget(playlistView);
	mainLayout->addLayout(bottomLayout);

	QWidget *central = new QWidget(this);
	central->setLayout(mainLayout);

	this->setCentralWidget(central);
	this->setFixedWidth(this->sizeHint().width());
    this->setWindowIcon(QIcon(":/icons/icons/music.png"));
	this->setWindowTitle("Simplicity");

	connect(player->getPlaylist(), SIGNAL(playlistChanged()), this, SLOT(playlistChanged()));
	connect(playlistView, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(playlistDoubleClicked(QListWidgetItem*)));
	connect(player, SIGNAL(updateProgress(qint64)), this, SLOT(updateProgressBar(qint64)));
	connect(player, SIGNAL(songChanged()), this, SLOT(currentlyPlayedSongChanged()));
	connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(songDurationChanged(qint64)));
	connect(songProgressBar, SIGNAL(pressedValue(qint64)), this, SLOT(progressBarClicked(qint64)));
	connect(shuffleButton, SIGNAL(toggled(bool)), this, SLOT(shuffleButtonToggled(bool)));
    connect(settingsButton, SIGNAL(clicked()), this, SLOT(openSettingsDialog()));
}

void MusicPlayerGui::loadPlaylist(QString playlistFileName)
{
	QFile saveFile(playlistFileName);
	if(saveFile.open(QIODevice::ReadOnly)) {
		QDataStream out(&saveFile);
		for(int i = 0; !out.atEnd(); i++) {
			QString fileName;
			out >> fileName;
			player->getPlaylist()->addSong(fileName);
		}
		saveFile.close();
    }
}

void MusicPlayerGui::setPlaylistLocation(QString newLocation)
{
    if(newLocation.isEmpty()) {
        qDebug() << "MusicPlayerGui: string with new playlist location is empty";
        newLocation = "."; //current folder
        QFileInfo info(newLocation);
        newLocation = info.absoluteFilePath(); //convert '.' to full (absolute) file path
    }
    playlistLocation = newLocation;
    qDebug() << "New playlist location: "<<newLocation;
}



