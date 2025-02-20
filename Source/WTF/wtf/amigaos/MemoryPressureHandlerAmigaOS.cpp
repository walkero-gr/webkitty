/*
 * Copyright (C) 2018 Sony Interactive Entertainment Inc.
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
#include <wtf/MemoryPressureHandler.h>
#if OS(AMIGAOS)
#define __USE_INLINE__
#endif
#include <proto/exec.h>
#if OS(AMIGAOS)
#undef __USE_INLINE__
#endif
#include <exec/memory.h>

#if !OS(AMIGAOS)
extern "C" { void dprintf(const char *,...); }
#endif

namespace WebKit {
	extern void reactOnMemoryPressureInWebKit();
}

namespace WTF {

void MemoryPressureHandler::platformReleaseMemory(Critical)
{
}

void MemoryPressureHandler::amigaosMeasurementTimerFired()
{
    setMemoryPressureStatus(MemoryPressureStatus::Normal);

	bool memoryLow = false;
	
	ULONG avail = AvailMem(MEMF_PUBLIC);
	ULONG largest = AvailMem(MEMF_LARGEST);
	ULONG total = AvailMem(MEMF_TOTAL);

	if (total > 950 * 1024 * 1024)
	{
		if (largest < 128 * 1024 * 1024)
			memoryLow = true;
		if (avail < 256 * 1024 * 1024)
			memoryLow = true;
	}
	else if (total > 450 * 1024 * 1024)
	{
		if (largest < 96 * 1024 * 1024)
			memoryLow = true;
		if (avail < 196 * 1024 * 1024)
			memoryLow = true;
	}
	else
	{
		if (largest < 48 * 1024 * 1024)
			memoryLow = true;
		if (avail < 96 * 1024 * 1024)
			memoryLow = true;
	}
	
	if (memoryLow)
	{
		setMemoryPressureStatus(MemoryPressureStatus::SystemCritical);
		releaseMemory(Critical::Yes);
		// TODO:This possible needs to be enabled when we build the WebKit fully
		// WebKit::reactOnMemoryPressureInWebKit();
		return;
	}
}

void MemoryPressureHandler::install()
{
    m_installed = true;
    m_amigaosMeasurementTimer.startRepeating(15_s);
}

void MemoryPressureHandler::uninstall()
{
    if (!m_installed)
        return;

    m_amigaosMeasurementTimer.stop();
    m_installed = false;
}

void MemoryPressureHandler::holdOff(Seconds)
{
}

void MemoryPressureHandler::respondToMemoryPressure(Critical critical, Synchronous synchronous)
{
    uninstall();

    releaseMemory(critical, synchronous);
}

std::optional<MemoryPressureHandler::ReliefLogger::MemoryUsage> MemoryPressureHandler::ReliefLogger::platformMemoryUsage()
{
    return std::nullopt;
}

} // namespace WTF
