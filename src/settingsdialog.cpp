#include "settingsdialog.h"


SettingsDialog::SettingsDialog(MusicPlayerGui *parent)
{
    this->setWindowTitle("Settings");
    this->setModal(true);
    playerGui = parent;
    QLabel *choosePlaylistLocationLabel = new QLabel("Choose where Simplicity should store playlist file:");
    choosePlaylistLocationButton = new QPushButton("Browse", this);
    locationLineEdit = new QLineEdit(this);
    locationLineEdit->setText(parent->getPlaylistLocation());
    locationLineEdit->setReadOnly(true);

    applyButton = new QPushButton("Apply");
    cancelButton = new QPushButton("Cancel");
    okButton = new QPushButton("Ok");

    QHBoxLayout *locationLayout = new QHBoxLayout();
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QVBoxLayout *mainSettingsLayout = new QVBoxLayout();

    locationLayout->addWidget(locationLineEdit);
    locationLayout->addWidget(choosePlaylistLocationButton);

    buttonsLayout->addWidget(applyButton);
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(okButton);

    mainSettingsLayout->addWidget(choosePlaylistLocationLabel);
    mainSettingsLayout->addLayout(locationLayout);
    mainSettingsLayout->addLayout(buttonsLayout);
    this->setLayout(mainSettingsLayout);


    connect(choosePlaylistLocationButton, SIGNAL(clicked()), this, SLOT(choosePlaylistLocationButtonPressed()));
    connect(applyButton, SIGNAL(clicked()), this, SLOT(applyButtonClicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));

}

void SettingsDialog::choosePlaylistLocationButtonPressed()
{
    QFileDialog foldersDialog;
    foldersDialog.setFileMode(QFileDialog::DirectoryOnly);
    foldersDialog.setOption(QFileDialog::DontUseNativeDialog, true);
    foldersDialog.setDirectory("/home/lukaszlinux");
    QListView *l = foldersDialog.findChild<QListView*>("listView");
    if (l) {
        l->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    QTreeView *t = foldersDialog.findChild<QTreeView*>();
    if (t) {
        t->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    QString folderName;
    if(foldersDialog.exec()) {
        folderName = foldersDialog.selectedFiles().first();
        setUserChosenPlaylistFolder(folderName);
        qDebug() << "Chosen folder: "<< folderName;
    }
}

void SettingsDialog::applyButtonClicked()
{
    if(!userChosenPlaylistFolder.isEmpty())
        this->playerGui->setPlaylistLocation(userChosenPlaylistFolder);
}

void SettingsDialog::cancelButtonClicked()
{
    this->close();
}

void SettingsDialog::okButtonClicked()
{
    this->applyButtonClicked();
    this->close();
}

void SettingsDialog::setUserChosenPlaylistFolder(QString folderName)
{
    userChosenPlaylistFolder = folderName;
    locationLineEdit->setText(userChosenPlaylistFolder);
}
