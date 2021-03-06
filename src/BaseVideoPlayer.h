#pragma once
#include "ofMain.h"
#include "ofxOMXPlayerSettings.h"

#include "LIBAV_INCLUDES.h"


#include "OMXReader.h"
#include "OMXClock.h"
#include "StreamInfo.h"
#include "OMXThread.h"
#include "BaseVideoDecoder.h"

#define MAX_DATA_SIZE 10 * 1024 * 1024


class BaseVideoPlayer: public OMXThread
{
public:
    BaseVideoPlayer();
    virtual ~BaseVideoPlayer();
    BaseVideoDecoder* decoder;
    std::deque<OMXPacket *> packets;
    
    bool isOpen;
    StreamInfo omxStreamInfo;
    double currentPTS;
    
    pthread_cond_t m_packet_cond;
    //pthread_cond_t m_picture_cond;
    pthread_mutex_t m_lock;
    pthread_mutex_t m_lock_decoder;
    
    OMXClock* omxClock;
    float fps;
    double frameTime;
    bool doAbort;
    bool doFlush;
    int speed;
    double timeStampAdjustment; // time stamp of last flippage. used to play at a forced framerate
    unsigned int cachedSize;
    
    
    void setSpeed(int speed);
    int getSpeed();
    
    virtual void close() = 0;
    bool decode(OMXPacket *pkt);
    void process();
    void flush();
    
    bool addPacket(OMXPacket *pkt);
    
    virtual bool openDecoder() =0;
    
    bool closeDecoder();
    double getCurrentPTS();
    double getFPS();
    
    unsigned int getCached();
    
    void submitEOS();
    bool EOS();
    
    void lock();
    void unlock();
    void lockDecoder();
    void unlockDecoder();
    
    
    uint32_t validHistoryPTS;
    bool doFlush_requested;
        
    int getCurrentFrame();
    void resetFrameCounter();
};