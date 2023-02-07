#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QAudioOutput;
class QMediaPlayer;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::Widget *ui;
    QList<QUrl> playList;
    QAudioOutput *audioOutput;
    QMediaPlayer *mediaPlayer;
    int index;
};
#endif // WIDGET_H
