#include "QtMediaPlayer.h"
#include "ui_QtMediaPlayer.h"
#include <QFileDialog>
#include <QDirIterator>

QtMediaPlayer::QtMediaPlayer(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::QtMediaPlayer),
    m_listModel(new QStandardItemModel(this)),
    m_player(new QMediaPlayer(this))
{
    ui->setupUi(this);
    ui->MusicList->setModel((m_listModel));

    m_player->setNotifyInterval(200);

    // 进度条连接信号
    connect(m_player, &QMediaPlayer::durationChanged,
            this, &QtMediaPlayer::updateDuration);
    connect(m_player, &QMediaPlayer::positionChanged,
            this, &QtMediaPlayer::updatePosition);

    // 音量连接信号
    ui->VolumeSlider->setValue(50);
    connect(ui->VolumeSlider, &QSlider::valueChanged,
            this, &QtMediaPlayer::on_VolumeSlider_valueChanged);

    // 把 ui 里现成的标签地址赋给成员
    m_Dur    = ui->Dur;
    m_CurDur = ui->CurDur;
    connect(m_player, &QMediaPlayer::durationChanged,
            this, &QtMediaPlayer::updateDuration);
    connect(m_player, &QMediaPlayer::positionChanged,
            this, &QtMediaPlayer::updatePosition);
}

QtMediaPlayer::~QtMediaPlayer()
{
    delete ui;
}

void QtMediaPlayer::playRow(int row)
{
    if (row < 0 || row >= m_listModel->rowCount()) return;

    QString file = m_listModel->item(row)->data(Qt::UserRole + 1).toString();
    if (file.isEmpty() || !m_player) return;

    m_currentRow = row;
    m_player->setMedia(QUrl::fromLocalFile(file));
    m_player->play();
}

void QtMediaPlayer::on_OpenDirBtn_clicked()
{
    // 打开文件夹
    auto path = QFileDialog::getExistingDirectory(this, "选择文件夹", "");
    if(path.isEmpty()){
        return;
    }

    // 打开目录, 列出媒体文件
    QDirIterator it(path, {"*.mp3", "*.wav"});
    while(it.hasNext()){
        auto info = it.next();
        auto item = new QStandardItem(it.fileName());

        // 存储媒体文件的完整路径
        item->setData(it.fileInfo().canonicalFilePath());
        qInfo() << info;
        m_listModel->appendRow(item);
    }
}

void QtMediaPlayer::on_MusicList_doubleClicked(const QModelIndex &index)
{
     qInfo() << "正在播放: " << index.data(Qt::UserRole+1).toString();
     QString file = index.data(Qt::UserRole+1).toString();
     m_player->setMedia(QUrl::fromLocalFile(file));
     m_player->play();
}

void QtMediaPlayer::on_PrevBtn_clicked()
{
    int row = m_currentRow;
    row = (row-1 + m_listModel->rowCount()) % m_listModel->rowCount();
    ui->MusicList->setCurrentIndex(m_listModel->index(row, 0));
    playRow(row);
}

void QtMediaPlayer::on_NextBtn_clicked()
{
    int row = m_currentRow;
    row = (row+1) % m_listModel->rowCount();
    ui->MusicList->setCurrentIndex(m_listModel->index(row, 0));
    playRow(row);
}

void QtMediaPlayer::on_PlayBtn_clicked()
{
    if(m_currentRow < 0 || m_currentRow >= m_listModel->rowCount())
        return;

    // 如果当前处于播放状态
    if(m_player->state() == QMediaPlayer::PlayingState){
        m_player->pause();
    } else {
        m_player->play();
    }
}

void QtMediaPlayer::on_PlaySlider_sliderMoved(int pos)
{
    m_player->setPosition(qint64(pos));
    m_CurDur->setText(formatTime(pos));
}

void QtMediaPlayer::updateDuration(qint64 dur)
{
    ui->PlaySlider->setRange(0, int(dur));
    m_Dur->setText(formatTime(dur));
}

void QtMediaPlayer::updatePosition(qint64 pos)
{
    if (!ui->PlaySlider->isSliderDown()){
        ui->PlaySlider->setValue(int(pos));
        ui->CurDur->setText(formatTime(pos));
    }
}

void QtMediaPlayer::on_VolumeSlider_valueChanged(int value)
{
    m_player->setVolume(value);
}

QString QtMediaPlayer::formatTime(qint64 ms)
{
    qint64 s = ms / 1000;
    qint64 m = s / 60;
    s %= 60;
    return QString("%1:%2").arg(m).arg(s, 2, 10, QChar('0'));
}
