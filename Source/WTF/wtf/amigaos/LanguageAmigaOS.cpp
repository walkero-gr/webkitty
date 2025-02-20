/*
 * Copyright (C) 2019 Sony Interactive Entertainment Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include <wtf/Language.h>
#include <wtf/HashMap.h>

#include <wtf/text/WTFString.h>

#include <proto/locale.h>
#include <libraries/locale.h>

namespace WTF {

Vector<String> platformUserPreferredLanguages(ShouldMinimizeLanguages)
{
	struct mapping { const char *locName; const char *code; };
	static const mapping mappings[] = {
		{ "dansk", "da" },
		{ "deutsch", "de" },
		{ "english", "en" },
		{ "espa\xF1ol", "sp" },
		{ "fran\xE7" "ais", "fr" },
		{ "greek", "gr" },
		{ "italiano", "it" },
		{ "magyar", "hu" },
		{ "nederlands", "nl" },
		{ "norsk", "no" },
		{ "polski", "pl" },
		{ "portugu\xEAs", "pt" },
		{ "suomi", "fi" },
		{ "svenska", "sv" },
		{ "t\xFCrkiye", "tr" },
		{ "\xE8" "e\xB9tina", "cs" }
	};
	Vector<String> languages;

	// TODO: This is temporary and needs to follow what the MorphOS version does
	// for now support only English
	languages.append("en");
	return languages;
}

} // namespace WTF
