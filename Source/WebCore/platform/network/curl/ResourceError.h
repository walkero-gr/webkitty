/*
 * Copyright (C) 2006 Apple Inc.  All rights reserved.
 * Copyright (C) 2017 Sony Interactive Entertainment Inc.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#pragma once

#include "ResourceErrorBase.h"
#include "CertificateInfo.h"
#include <optional>

namespace WebCore {

class ResourceError : public ResourceErrorBase {
    friend class ResourceErrorBase;

public:
    ResourceError(Type type = Type::Null)
        : ResourceErrorBase(type)
    {
    }

    ResourceError(const String& domain, int errorCode, const URL& failingURL, const String& localizedDescription, Type type = Type::General, IsSanitized isSanitized = IsSanitized::No)
        : ResourceErrorBase(domain, errorCode, failingURL, localizedDescription, type, isSanitized)
    {
    }

    WEBCORE_EXPORT static ResourceError httpError(int errorCode, const URL& failingURL, Type = Type::General);
    static ResourceError sslError(int errorCode, unsigned sslErrors, const URL& failingURL);

    unsigned sslErrors() const { return m_sslErrors; }
    void setSslErrors(unsigned sslErrors) { m_sslErrors = sslErrors; }

    bool isSSLConnectError() const;
    WEBCORE_EXPORT bool isSSLCertVerificationError() const;

    static bool platformCompare(const ResourceError& a, const ResourceError& b);

    std::optional<CertificateInfo> certificateInfo() const { return m_certificateInfo; }
    void setCertificateInfo(CertificateInfo&&info) { m_certificateInfo = WTFMove(info); };

private:
    void doPlatformIsolatedCopy(const ResourceError&);

    static const char* const curlErrorDomain;

    unsigned m_sslErrors { 0 };
	
    std::optional<CertificateInfo> m_certificateInfo;
};

} // namespace WebCore
