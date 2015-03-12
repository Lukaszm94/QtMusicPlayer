#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "musicplayerGUI.h"

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


#endif // SETTINGSDIALOG_H
