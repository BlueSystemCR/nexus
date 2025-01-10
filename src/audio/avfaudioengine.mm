#import <AVFoundation/AVFoundation.h>
#import <AppKit/NSWorkspace.h>
#include "avfaudioengine.h"
#include <QDebug>
#include <QUrl>

#ifdef Q_OS_MACOS

@interface AVFAudioEngineDelegate : NSObject
@property (nonatomic, assign) AVFAudioEngine* engine;
@property (nonatomic, assign) AVFAudioEngineImpl* impl;
- (void)handleInterruption:(NSNotification*)notification;
@end

@implementation AVFAudioEngineDelegate

- (void)handleInterruption:(NSNotification*)notification {
    // En macOS, usamos NSWorkspaceWillSleepNotification y NSWorkspaceDidWakeNotification
    NSString* name = [notification name];
    if ([name isEqualToString:NSWorkspaceWillSleepNotification]) {
        [(id)self.impl pause];
    } else if ([name isEqualToString:NSWorkspaceDidWakeNotification]) {
        [(id)self.impl resume];
    }
}

@end

class AVFAudioEngineImpl {
public:
    AVFAudioEngine* owner;
    AVAudioEngine* engine;
    AVAudioPlayerNode* playerNode;
    AVAudioUnitEQ* eqNode;
    AVAudioUnitTimePitch* pitchNode;
    AVAudioFile* audioFile;
    AVFAudioEngineDelegate* delegate;
    bool isPlaying;
    float currentTime;
    float duration;

    AVFAudioEngineImpl(AVFAudioEngine* owner) 
        : owner(owner), engine(nil), playerNode(nil), eqNode(nil), pitchNode(nil),
          audioFile(nil), delegate(nil), isPlaying(false), currentTime(0), duration(0) {
        setupAudioEngine();
        setupWorkspaceNotifications();
    }

    ~AVFAudioEngineImpl() {
        cleanup();
    }

    void cleanup() {
        if (engine) {
            [engine stop];
        }
        
        if (delegate) {
            [[NSNotificationCenter defaultCenter] removeObserver:delegate];
            [delegate release];
        }
        
        [engine release];
        [playerNode release];
        [eqNode release];
        [pitchNode release];
        [audioFile release];
    }

    void setupWorkspaceNotifications() {
        delegate = [[AVFAudioEngineDelegate alloc] init];
        delegate.engine = owner;
        delegate.impl = this;
        
        NSNotificationCenter* center = [[NSWorkspace sharedWorkspace] notificationCenter];
        [center addObserver:delegate
                 selector:@selector(handleInterruption:)
                     name:NSWorkspaceWillSleepNotification
                   object:nil];
        [center addObserver:delegate
                 selector:@selector(handleInterruption:)
                     name:NSWorkspaceDidWakeNotification
                   object:nil];
    }

    void setupAudioEngine() {
        engine = [[AVAudioEngine alloc] init];
        playerNode = [[AVAudioPlayerNode alloc] init];
        eqNode = [[AVAudioUnitEQ alloc] initWithNumberOfBands:10];
        pitchNode = [[AVAudioUnitTimePitch alloc] init];
        
        [engine attachNode:playerNode];
        [engine attachNode:eqNode];
        [engine attachNode:pitchNode];
        
        AVAudioFormat* format = [[AVAudioFormat alloc] initStandardFormatWithSampleRate:44100 channels:2];
        [engine connect:playerNode to:eqNode format:format];
        [engine connect:eqNode to:pitchNode format:format];
        [engine connect:pitchNode to:engine.mainMixerNode format:format];
        [format release];
        
        NSError* error = nil;
        if (![engine startAndReturnError:&error]) {
            qWarning() << "Failed to start audio engine:" << QString::fromNSString(error.localizedDescription);
        }
    }

    bool play(const QString& filePath) {
        NSError* error = nil;
        NSURL* url = [NSURL fileURLWithPath:filePath.toNSString()];
        
        AVAudioFile* newFile = [[AVAudioFile alloc] initForReading:url error:&error];
        if (error) {
            emit owner->errorOccurred(QString::fromNSString(error.localizedDescription));
            [newFile release];
            return false;
        }
        
        if (audioFile) {
            [audioFile release];
        }
        audioFile = newFile;
        
        duration = audioFile.length / audioFile.processingFormat.sampleRate;
        emit owner->durationChanged(duration * 1000);
        
        [playerNode scheduleFile:audioFile atTime:nil completionCallbackType:AVAudioPlayerNodeCompletionDataPlayedBack completionHandler:^(AVAudioPlayerNodeCompletionCallbackType callbackType) {
            emit owner->playbackFinished();
        }];
        
        [playerNode play];
        isPlaying = true;
        emit owner->playbackStarted();
        
        [NSTimer scheduledTimerWithTimeInterval:0.1
                                      repeats:YES
                                        block:^(NSTimer* timer) {
            if (isPlaying) {
                float position = playerNode.lastRenderTime.sampleTime / audioFile.processingFormat.sampleRate;
                emit owner->positionChanged(position * 1000);
            }
        }];
        
        return true;
    }

    void pause() {
        if (isPlaying) {
            [playerNode pause];
            isPlaying = false;
            emit owner->playbackPaused();
        }
    }

    void resume() {
        if (!isPlaying) {
            [playerNode play];
            isPlaying = true;
            emit owner->playbackStarted();
        }
    }

    void stop() {
        if (isPlaying) {
            [playerNode stop];
            isPlaying = false;
            emit owner->playbackStopped();
        }
    }

    void seek(qint64 position) {
        float seconds = position / 1000.0f;
        AVAudioFramePosition framePosition = seconds * audioFile.processingFormat.sampleRate;
        
        [playerNode stop];
        
        AVAudioTime* startTime = [AVAudioTime timeWithSampleTime:framePosition
                                                        atRate:audioFile.processingFormat.sampleRate];
        
        [playerNode scheduleFile:audioFile
                        atTime:startTime
                        completionCallbackType:AVAudioPlayerNodeCompletionDataPlayedBack
                        completionHandler:^(AVAudioPlayerNodeCompletionCallbackType callbackType) {
            emit owner->playbackFinished();
        }];
        
        if (isPlaying) {
            [playerNode play];
        }
    }

    void setVolume(float volume) {
        playerNode.volume = volume;
    }

    void setEQBand(int band, float gain) {
        if (band >= 0 && band < 10) { // EQ tiene 10 bandas fijas
            eqNode.bands[band].gain = gain;
        }
    }

    void setPitch(float cents) {
        pitchNode.pitch = cents;
    }

    float getCurrentTime() const {
        if (!audioFile) return 0;
        return playerNode.lastRenderTime.sampleTime / audioFile.processingFormat.sampleRate;
    }

    float getDuration() const {
        return duration;
    }
};

AVFAudioEngine::AVFAudioEngine(QObject* parent)
    : QObject(parent), d(new AVFAudioEngineImpl(this)) {
}

AVFAudioEngine::~AVFAudioEngine() = default;

bool AVFAudioEngine::initialize() {
    return true;
}

void AVFAudioEngine::cleanup() {
    d->cleanup();
}

bool AVFAudioEngine::play(const QString& filePath) {
    return d->play(filePath);
}

void AVFAudioEngine::pause() {
    d->pause();
}

void AVFAudioEngine::resume() {
    d->resume();
}

void AVFAudioEngine::stop() {
    d->stop();
}

void AVFAudioEngine::seek(qint64 position) {
    d->seek(position);
}

void AVFAudioEngine::setVolume(float volume) {
    d->setVolume(volume);
}

void AVFAudioEngine::setEQBand(int band, float gain) {
    d->setEQBand(band, gain);
}

void AVFAudioEngine::setPitch(float cents) {
    d->setPitch(cents);
}

float AVFAudioEngine::getCurrentTime() const {
    return d->getCurrentTime();
}

float AVFAudioEngine::getDuration() const {
    return d->getDuration();
}

#endif // Q_OS_MACOS
