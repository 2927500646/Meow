#ifndef QTMEDIAPLAYER_H
#define QTMEDIAPLAYER_H

#include <QWidget>
#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QLabel>


namespace Ui {
class QtMediaPlayer;
}

class QtMediaPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit QtMediaPlayer(QWidget *parent = 0);
    ~QtMediaPlayer();

private slots:
    void on_OpenDirBtn_clicked();

    void on_MusicList_doubleClicked(const QModelIndex &index);

    void on_PrevBtn_clicked();

    void on_NextBtn_clicked();

    void on_PlayBtn_clicked();

    void on_PlaySlider_sliderMoved(int position);

    void updatePosition(qint64 pos);

    void updateDuration(qint64 dur);

    void on_VolumeSlider_valueChanged(int value);

private:
    void playRow(int row);
    Ui::QtMediaPlayer *ui;
    QStandardItemModel* m_listModel;\
    QMediaPlayer *m_player;
    int m_currentRow = -1;   // 当前播放行号

    bool m_sliderPressed = false;

    QLabel *m_Dur;   // 该歌曲播放时刻
    QLabel *m_CurDur;// 当前播放时刻

    QString formatTime(qint64 ms);
};

#endif // QTMEDIAPLAYER_H
