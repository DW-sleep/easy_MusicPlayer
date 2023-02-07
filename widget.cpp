#include "widget.h"
#include "ui_widget.h"
#include<QDebug>
#include<QFileDialog>
#include<QDir>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QUrl>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //播放音乐基本操作
    audioOutput = new QAudioOutput(this);
    mediaPlayer = new QMediaPlayer(this);

    //将output对象添加到媒体播放器中
    mediaPlayer->setAudioOutput(audioOutput);

    //给播放器设置音乐
    //mediaPlayer->setSource(QUrl::fromLocalFile("D:/Qt_Test/MusicPlayer/music/sapientdream - Promise.mp3"));
    //开始播放
    //mediaPlayer->play();

    //获取当前媒体的时长
    connect(mediaPlayer,&QMediaPlayer::durationChanged,this,[=](qint64 duration)
    {
        ui->totalLabel->setText(QString("%1:%2").arg(duration/1000/60).arg(duration/1000%60));
        ui->playCourseSlider->setRange(0,duration);
    });

    //获取当前播放时长
    connect(mediaPlayer,&QMediaPlayer::positionChanged,this,[=](qint64 pos)
    {
        ui->curLabel->setText(QString("%1:%2").arg(pos/1000/60).arg(pos/1000%60));
        ui->playCourseSlider->setValue(pos);
    });

    //拖动进度条
    connect(ui->playCourseSlider,&QSlider::sliderMoved,mediaPlayer,&QMediaPlayer::setPosition);


}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    auto path = QFileDialog::getExistingDirectory(this,"请选择音乐所在的目录","D:/Qt_Test/MusicPlayer/music");
    //获取路径中的mp3文件
    QDir dir(path);
    auto musicList = dir.entryList(QStringList()<<"*.mp3");
    ui->listWidget->addItems(musicList);
    //ui->listWidget->setCurrentRow(0);

    //把音乐的完整路径保存起来
    for(auto file : musicList)
    {
        playList.append(QUrl::fromLocalFile(path+"/"+file));
    }
    qInfo()<<playList;
}

//暂停和播放功能
void Widget::on_pushButton_4_clicked()
{
    if(playList.empty())
    {
        return;
    }

    switch(mediaPlayer->playbackState())
    {
        case QMediaPlayer::PlaybackState::StoppedState:  //停止状态
        {
            //播放当前选中的音乐
            //获取选中的行号
            index = ui->listWidget->currentRow();
            //播放对应下标的音乐
            mediaPlayer->setSource(playList[index]);
            mediaPlayer->play();
        }
            break;

        case QMediaPlayer::PlaybackState::PlayingState:  //播放状态
            mediaPlayer->pause();
            break;
        case QMediaPlayer::PlaybackState::PausedState:  //暂停状态
            mediaPlayer->play();
            break;
    }

    /*
    //播放当前选中的音乐
    //获取选中的行号
    int index = ui->listWidget->currentRow();
    //播放对应下标的音乐
    mediaPlayer->setSource(playList[index]);
    mediaPlayer->play();
    */
}

//下一首
void Widget::on_pushButton_5_clicked()
{
    ++index;
    //如果越界，回到第一首歌
    if(index>playList.size())
    {
        index = 0;
    }

    ui->listWidget->setCurrentRow(index);
    mediaPlayer->setSource(playList[index]);
}

//上一首
void Widget::on_pushButton_3_clicked()
{
    --index;
    //如果越界，回到最后一首歌
    if(index<0)
    {
        index = playList.size()-1;
    }

    ui->listWidget->setCurrentRow(index);
    mediaPlayer->setSource(playList[index]);
}


void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{

    mediaPlayer->setSource(playList[index.row()]);
    mediaPlayer->play();
}

