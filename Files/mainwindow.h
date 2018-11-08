#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QSlider>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QMediaPlayer *player;
    QVideoWidget *vw;
    QSlider *slider;

private slots:
    void on_pushButton_selectVideo_clicked();

    void on_pushButton_selectFile_clicked();

    void on_pushButton_Open_clicked();

    void on_actionPlay_triggered();

    void on_actionPause_triggered();

    void on_actionStop_triggered();

    void on_pushButton_prevWord_clicked();

    void on_pushButton_Next_clicked();

    void on_pushButton_framePath_clicked();

    void on_pushButton_saveFrames_clicked();

    void on_actionPrevious_triggered();

    void on_actionNext_triggered();

    void on_actionClose_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
