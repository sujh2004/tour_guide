#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <QObject>
#include <QAudioOutput>

class MusicPlayer : public QObject
{
    Q_OBJECT

public:
    explicit MusicPlayer(QObject *parent = nullptr);
    ~MusicPlayer();

    void playMusic(const QString &filePath);  // 播放音乐
    void stopMusic();                         // 停止播放
    void setLooping(bool loop);               // 设置是否循环播放
    void pauseMusic();                        // 暂停播放
    void resumeMusic();                       // 恢复播放

private slots:
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);  // 状态变化槽函数

private:
    QMediaPlayer *player;
    bool loopEnabled = false;
    QAudioOutput *audioOutput;
};

#endif // MUSICPLAYER_H
