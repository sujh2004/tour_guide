#include "musicplayer.h"
#include <QUrl>
#include <QDebug>


MusicPlayer::MusicPlayer(QObject *parent)
    : QObject(parent), player(new QMediaPlayer(this))
{
    audioOutput = new QAudioOutput(this);
    player = new QMediaPlayer(this);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);
    connect(player, &QMediaPlayer::playbackStateChanged, this, &MusicPlayer::onPlaybackStateChanged);
}

MusicPlayer::~MusicPlayer()
{
    delete player;
}

void MusicPlayer::playMusic(const QString &filePath)
{
    player->setSource(QUrl(filePath));  // Qt 6 使用 setSource
    player->play();
}

void MusicPlayer::stopMusic()
{
    player->stop();
}

void MusicPlayer::pauseMusic()
{
    player->pause();
}

void MusicPlayer::resumeMusic()
{
    player->play();
}

void MusicPlayer::setLooping(bool loop)
{
    loopEnabled = loop;
}

void MusicPlayer::onPlaybackStateChanged(QMediaPlayer::PlaybackState state)
{
    if (state == QMediaPlayer::StoppedState && loopEnabled) {
        player->play();  // 手动重新播放实现循环
    }
}
