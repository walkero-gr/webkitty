#include "MediaDescriptionAmigaOS.h"

#if ENABLE(VIDEO) && ENABLE(MEDIA_SOURCE)

namespace WebCore {

MediaDescriptionAmigaOS::MediaDescriptionAmigaOS(MediaDescriptionAmigaOS::Type type, const AtomString &codec)
	: m_codec(codec)
	, m_type(type)
{
}

RefPtr<MediaDescription> MediaDescriptionAmigaOS::createVideoWithCodec(const AtomString &codec)
{
	return adoptRef(*new MediaDescriptionAmigaOS(Type::Video, codec));
}

RefPtr<MediaDescription> MediaDescriptionAmigaOS::createAudioWithCodec(const AtomString &codec)
{
	return adoptRef(*new MediaDescriptionAmigaOS(Type::Audio, codec));
}

RefPtr<MediaDescription> MediaDescriptionAmigaOS::createTextWithCodec(const AtomString &codec)
{
	return adoptRef(*new MediaDescriptionAmigaOS(Type::Text, codec));
}

}

#endif
