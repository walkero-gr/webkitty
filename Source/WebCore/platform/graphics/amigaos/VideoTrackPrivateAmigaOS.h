#pragma once

#if ENABLE(VIDEO)

#include "VideoTrackPrivate.h"
#include "MediaPlayerPrivateAmigaOS.h"
#include "MediaSourceBufferPrivateAmigaOS.h"
#include <wtf/WeakPtr.h>

namespace WebCore {

class VideoTrackPrivateAmigaOS : public VideoTrackPrivate
{
public:

    static RefPtr<VideoTrackPrivateAmigaOS> create(WeakPtr<MediaPlayerPrivateAmigaOS> player, int index)
    {
        return adoptRef(*new VideoTrackPrivateAmigaOS(player, index));
    }

    Kind kind() const final;
	void setSelected(bool selected) override;
    virtual void disconnect();

    int trackIndex() const override { return m_index; }

    AtomString id() const override { return AtomString(m_id); }
    AtomString label() const override { return AtomString(m_label); }
    AtomString language() const override { return AtomString(m_language); }

protected:
    VideoTrackPrivateAmigaOS(WeakPtr<MediaPlayerPrivateAmigaOS>, int index);

	int m_index;
    String m_id;
    String m_label;
    String m_language;
    WeakPtr<MediaPlayerPrivateAmigaOS> m_player;
};

#if ENABLE(MEDIA_SOURCE)
class VideoTrackPrivateAmigaOSMS final : public VideoTrackPrivateAmigaOS
{
public:

    static RefPtr<VideoTrackPrivateAmigaOSMS> create(MediaSourceBufferPrivateAmigaOS* source, int index)
    {
        return adoptRef(*new VideoTrackPrivateAmigaOSMS(source, index));
    }

	void setSelected(bool selected) override;
    void disconnect() override;

private:
    VideoTrackPrivateAmigaOSMS(MediaSourceBufferPrivateAmigaOS *source, int index);
    MediaSourceBufferPrivateAmigaOS *m_source;
};
#endif

}

#endif
