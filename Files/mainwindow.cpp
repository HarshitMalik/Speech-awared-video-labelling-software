#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "write_subtitles.h"
#include "frames.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QBoxLayout>
#include <QToolBar>


class words_info
{
public:
    QString word;
    QString end_time;
    QString start_time;

};
QVector<words_info>Vector;
int ind=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QToolBar *toolBar2 = new QToolBar();
    addToolBarBreak();
    toolBar2=addToolBar("toolBar2");
    QToolBar *toolBar3 = new QToolBar();
    addToolBarBreak();
    toolBar3=addToolBar("toolBar3");

    player = new QMediaPlayer;
    vw = new QVideoWidget;
    player->setVideoOutput(vw);
    vw->setMinimumSize(300,225);
    vw->setMaximumHeight(250);
    slider = new QSlider;
    slider->setOrientation(Qt::Horizontal);

    toolBar2->addWidget(vw);
    toolBar3->addWidget(slider);
    connect(player,&QMediaPlayer::durationChanged,slider,&QSlider::setMaximum);
    connect(player,&QMediaPlayer::positionChanged,slider,&QSlider::setValue);
    connect(slider,&QSlider::sliderMoved,player,&QMediaPlayer::setPosition);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_pushButton_selectVideo_clicked()
{
    QString videoPath = QFileDialog::getOpenFileName(this,"Select File",QDir::homePath());
    ui->lineEdit_videoPath->setText(videoPath);
    ui->statusBar->showMessage("Video Added :"+videoPath,1500);
}

void MainWindow::on_pushButton_selectFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,"Select File",QDir::homePath(),"*.txt");
    ui->lineEdit_filePath->setText(filePath);
    ui->statusBar->showMessage("File Added :"+filePath,1500);
}

void MainWindow::on_pushButton_Open_clicked()
{
    QString pathVideo = ui->lineEdit_videoPath->text();
    QString pathFile = ui->lineEdit_filePath->text();
    if(pathVideo==NULL||pathFile==NULL)
        QMessageBox::warning(this,"Error","No text file or video file selected !");
    else
    {

     on_actionStop_triggered();
     player->setMedia(QUrl::fromLocalFile(pathVideo));
     ui->statusBar->showMessage("Calling Speech to text API....");
     writesubs(pathVideo.toStdString(),pathFile.toStdString()); //Function calling Google API using python script
     QFile file(ui->lineEdit_filePath->text());
     if(!(file.open(QFile::ReadOnly|QFile::Text)))
      {
        QMessageBox::warning(this,"Error","File can not be added !");
        return ;
       }

     QTextStream in(&file);
     QString text=in.readAll();
     ui->statusBar->showMessage("Text file written succesfully ! ",1500);
     ui->plainTextEdit_txtFile->setPlainText(text);
     file.close();

     words_info container;
     QFile file2(pathFile);
     file2.open(QFile::ReadOnly | QFile::Text);
     QTextStream in2(&file2);
     in2.seek(0);
     QString line;
     ui->statusBar->showMessage("Please Wait....reading file ! ");
          while (!in2.atEnd()) {
              line = in2.readLine();
              line = in2.readLine();
              container.start_time=line;
              line = in2.readLine();
              container.end_time=line;
              line = in2.readLine();
              container.word=line;
              line = in2.readLine();
              Vector.append(container);

          }

         ui->lineEdit_wordDetected->setText(Vector.at(0).word);
         ui->lineEdit_from->setText(Vector.at(0).start_time);
         ui->lineEdit_to->setText(Vector.at(0).end_time);
         float f = (Vector.at(0).end_time).toFloat()-(Vector.at(0).start_time).toFloat();
         ui->lineEdit_duration->setText(QString::number(f));
         ui->statusBar->showMessage("Text file reading succesful ! ",1500);
          on_actionPlay_triggered();
    }
}

void MainWindow::on_actionPlay_triggered()
{
    player->play();
    ui->statusBar->showMessage("Video playing",1000);
}

void MainWindow::on_actionPause_triggered()
{
    player->pause();
    ui->statusBar->showMessage("Video paused");
}

void MainWindow::on_actionStop_triggered()
{
    player->stop();
    ui->statusBar->showMessage("Video stopped");
}

void MainWindow::on_pushButton_prevWord_clicked()
{
    if(ui->lineEdit_wordDetected->text()==NULL)
       QMessageBox::information(this,"Word","No word to display");
    else
    {
     ind--;
     if(ind>=0){
     ui->lineEdit_wordDetected->setText(Vector.at(ind).word);
     ui->lineEdit_from->setText(Vector.at(ind).start_time);
     ui->lineEdit_to->setText(Vector.at(ind).end_time);
     float f = (Vector.at(ind).end_time).toFloat()-(Vector.at(ind).start_time).toFloat();
     ui->lineEdit_duration->setText(QString::number(f));
     player->setPosition((Vector.at(ind).start_time).toFloat()*1000);
     player->play();
     ui->statusBar->showMessage("Previous word selected",1000);
    }
    else{
     ind++;
     QMessageBox::warning(this,"Error",Vector.at(ind).word+" is the first word");
     ui->statusBar->showMessage("Error: No previous word exists",1000);
    }
   }
}

void MainWindow::on_pushButton_Next_clicked()
{
    if(ui->lineEdit_wordDetected->text()==NULL)
       QMessageBox::information(this,"Word","No word to display");
    else
    {
     ind++;
     if(ind<Vector.size()){
     ui->lineEdit_wordDetected->setText(Vector.at(ind).word);
     ui->lineEdit_from->setText(Vector.at(ind).start_time);
     ui->lineEdit_to->setText(Vector.at(ind).end_time);
     float f = (Vector.at(ind).end_time).toFloat()-(Vector.at(ind).start_time).toFloat();
     ui->lineEdit_duration->setText(QString::number(f));
     player->setPosition((Vector.at(ind).start_time).toFloat()*1000);
     player->play();
     ui->statusBar->showMessage("Next word selected",1000);
     }
     else{
        ind--;
        QMessageBox::warning(this,"Error",Vector.at(ind).word+" is the last word");
        ui->statusBar->showMessage("Error: No next word exists",1000);
    }
   }
}

void MainWindow::on_pushButton_framePath_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),QDir::homePath(),QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    ui->lineEdit_framePath->setText(dir);
}

void MainWindow::on_pushButton_saveFrames_clicked()
{
    if(ui->lineEdit_filePath->text()==NULL||ui->lineEdit_videoPath->text()==NULL)
        QMessageBox::information(this,"Error","No text file or video file seleceted !");
    else if(ui->lineEdit_wordDetected->text()==NULL)
        QMessageBox::information(this,"Error","No Video is opened !");
    else if(ui->lineEdit_framePath->text()==NULL)
          QMessageBox::information(this,"Error","No output folder seleceted !");
    else{
    saveframes((ui->lineEdit_videoPath->text()).toStdString(),(ui->lineEdit__noOfFrames->text()).toStdString(),(ui->lineEdit_from->text()).toStdString(),(ui->lineEdit_wordDetected->text()).toStdString(),(ui->lineEdit_framePath->text()).toStdString(),(ui->lineEdit__frameDiff->text()).toDouble());
    ui->statusBar->showMessage("Frames saved succesfully",1500);
    }
}

void MainWindow::on_actionPrevious_triggered()
{
    on_pushButton_prevWord_clicked();
}

void MainWindow::on_actionNext_triggered()
{
 on_pushButton_Next_clicked();
}

void MainWindow::on_actionClose_triggered()
{
    QApplication::quit();
}
