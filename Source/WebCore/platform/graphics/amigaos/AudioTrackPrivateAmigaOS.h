#pragma once

#if ENABLE(VIDEO)

#include "AudioTrackPrivate.h"
#include "MediaPlayerPrivateAmigaOS.h"
#include "MediaSourceBufferPrivateAmigaOS.h"
#include <wtf/WeakPtr.h>

namespace WebCore {

class AudioTrackPrivateAmigaOS : public AudioTrackPrivate
{
public:

    static RefPtr<AudioTrackPrivateAmigaOS> create(WeakPtr<MediaPlayerPrivateAmigaOS> player, int index)
    {
        return adoptRef(*new AudioTrackPrivateAmigaOS(player, index));
    }

    Kind kind() const final;

    virtual void disconnect();

    void setEnabled(bool) override;

    int trackIndex() const override { return m_index; }

    AtomString id() const override { return AtomString(m_id); }
    AtomString label() const override { return AtomString(m_label); }
    AtomString language() const override { return AtomString(m_language); }

protected:
    AudioTrackPrivateAmigaOS(WeakPtr<MediaPlayerPrivateAmigaOS>, int index);

	int m_index;
    String m_id;
    String m_label;
    String m_language;
    WeakPtr<MediaPlayerPrivateAmigaOS> m_player;
};

#if ENABLE(MEDIA_SOURCE)
class AudioTrackPrivateAmigaOSMS : public AudioTrackPrivateAmigaOS
{
public:

    static RefPtr<AudioTrackPrivateAmigaOSMS> create(MediaSourceBufferPrivateAmigaOS *source, int index)
    {
        return adoptRef(*new AudioTrackPrivateAmigaOSMS(source, index));
    }

    void disconnect() override;
    void setEnabled(bool) override;

protected:
    AudioTrackPrivateAmigaOSMS(MediaSourceBufferPrivateAmigaOS*, int index);
    MediaSourceBufferPrivateAmigaOS *m_source;
};
#endif

}

#endif
