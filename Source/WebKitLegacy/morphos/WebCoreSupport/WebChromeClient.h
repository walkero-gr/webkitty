/*
 * Copyright (C) 2006-2017 Apple Inc. All rights reserved.
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "WebKit.h"
#include <WebCore/ChromeClient.h>
#include <WebCore/GraphicsContext.h>
#include <WebCore/FocusDirection.h>
#include <WebCore/ScrollTypes.h>
#include <wtf/Forward.h>
#include <wtf/RefPtr.h>

namespace WebKit {

class WebPage;
class WebDesktopNotificationsDelegate;

class WebChromeClient final : public WebCore::ChromeClient {
public:
    WebChromeClient(WebKit::WebPage&);

    WebPage* webPage() const { return &m_webPage; }
    WebPage& page() const { return m_webPage; }

protected:
    void chromeDestroyed() final;

    void setWindowRect(const WebCore::FloatRect&) final;
    WebCore::FloatRect windowRect() final;
    
    WebCore::FloatRect pageRect() final;

    void focus() final;
    void unfocus() final;

    bool canTakeFocus(WebCore::FocusDirection) final;
    void takeFocus(WebCore::FocusDirection) final;

    void focusedElementChanged(WebCore::Element*) final;
    void focusedFrameChanged(WebCore::Frame*) final;

    WebCore::Page* createWindow(WebCore::Frame&, const WebCore::WindowFeatures&, const WebCore::NavigationAction&) final;
    void show() final;

    bool canRunModal() final;
    void runModal() final;

    void setToolbarsVisible(bool) final;
    bool toolbarsVisible() final;
    
    void setStatusbarVisible(bool) final;
    bool statusbarVisible() final;
    
    void setScrollbarsVisible(bool) final;
    bool scrollbarsVisible() final;
    
    void setMenubarVisible(bool) final;
    bool menubarVisible() final;

    void setResizable(bool) final;

    void addMessageToConsole(JSC::MessageSource, JSC::MessageLevel, const WTF::String& message, unsigned lineNumber, unsigned columnNumber, const WTF::String& url) final;

    bool canRunBeforeUnloadConfirmPanel() final;
    bool runBeforeUnloadConfirmPanel(const WTF::String& message, WebCore::Frame&) final;

    void closeWindow() final;

    void runJavaScriptAlert(WebCore::Frame&, const WTF::String&) final;
    bool runJavaScriptConfirm(WebCore::Frame&, const WTF::String&) final;
    bool runJavaScriptPrompt(WebCore::Frame&, const WTF::String& message, const WTF::String& defaultValue, WTF::String& result) final;
    void setStatusbarText(const WTF::String&) final;

    WebCore::KeyboardUIMode keyboardUIMode() final;

    void invalidateRootView(const WebCore::IntRect&) final;
    void invalidateContentsAndRootView(const WebCore::IntRect&) final;
    void invalidateContentsForSlowScroll(const WebCore::IntRect&) final;
    void scroll(const WebCore::IntSize& scrollDelta, const WebCore::IntRect& rectToScroll, const WebCore::IntRect& clipRect) final;

    WebCore::IntPoint screenToRootView(const WebCore::IntPoint&) const final;
    WebCore::IntRect rootViewToScreen(const WebCore::IntRect&) const final;
    WebCore::IntPoint accessibilityScreenToRootView(const WebCore::IntPoint&) const final;
    WebCore::IntRect rootViewToAccessibilityScreen(const WebCore::IntRect&) const final;
    PlatformPageClient platformPageClient() const final;
    void contentsSizeChanged(WebCore::Frame&, const WebCore::IntSize&) const final;
    void intrinsicContentsSizeChanged(const WebCore::IntSize&) const final;

    void mouseDidMoveOverElement(const WebCore::HitTestResult&, unsigned modifierFlags, const WTF::String& toolTip, WebCore::TextDirection) final;
    bool shouldUnavailablePluginMessageBeButton(WebCore::RenderEmbeddedObject::PluginUnavailabilityReason) const final;
    void unavailablePluginButtonClicked(WebCore::Element&, WebCore::RenderEmbeddedObject::PluginUnavailabilityReason) const final;

    void print(WebCore::Frame&, const WebCore::StringWithDirection&) final;

    void exceededDatabaseQuota(WebCore::Frame&, const WTF::String&, WebCore::DatabaseDetails) final;

    void reachedMaxAppCacheSize(int64_t spaceNeeded) final;
    void reachedApplicationCacheOriginQuota(WebCore::SecurityOrigin&, int64_t totalSpaceNeeded) final;

    void runOpenPanel(WebCore::Frame&, WebCore::FileChooser&) final;
    void loadIconForFiles(const Vector<WTF::String>&, WebCore::FileIconLoader&) final;

    void setCursor(const WebCore::Cursor&) final;
    void setCursorHiddenUntilMouseMoves(bool) final;

    // Pass 0 as the GraphicsLayer to detatch the root layer.
    void attachRootGraphicsLayer(WebCore::Frame&, WebCore::GraphicsLayer*) final;
    void attachViewOverlayGraphicsLayer(WebCore::GraphicsLayer*) final;
    // Sets a flag to specify that the next time content is drawn to the window,
    // the changes appear on the screen in synchrony with updates to GraphicsLayers.
    void setNeedsOneShotDrawingSynchronization() final { }
    void triggerRenderingUpdate() final;
		
 	CompositingTriggerFlags allowedCompositingTriggers() const { return static_cast<CompositingTriggerFlags>(0); }
#if 0
    CompositingTriggerFlags allowedCompositingTriggers() const final
    {
        return static_cast<CompositingTriggerFlags>(
            ThreeDTransformTrigger |
            VideoTrigger |
            PluginTrigger|
            CanvasTrigger |
            AnimationTrigger);
    }
#endif

    bool hoverSupportedByPrimaryPointingDevice() const final { return true; };
    bool hoverSupportedByAnyAvailablePointingDevice() const final { return true; }
    std::optional<WebCore::PointerCharacteristics> pointerCharacteristicsOfPrimaryPointingDevice() const final { return WebCore::PointerCharacteristics::Fine; }
    OptionSet<WebCore::PointerCharacteristics> pointerCharacteristicsOfAllAvailablePointingDevices() const final { return WebCore::PointerCharacteristics::Fine; }

    void scrollContainingScrollViewsToRevealRect(const WebCore::IntRect&) const final { }

	void setTextIndicator(const WebCore::TextIndicatorData&) const final { }

    bool selectItemWritingDirectionIsNatural() final;
    bool selectItemAlignmentFollowsMenuWritingDirection() final;
    RefPtr<WebCore::PopupMenu> createPopupMenu(WebCore::PopupMenuClient&) const final;
    RefPtr<WebCore::SearchPopupMenu> createSearchPopupMenu(WebCore::PopupMenuClient&) const final;

#if ENABLE(FULLSCREEN_API)
    bool supportsFullScreenForElement(const WebCore::Element&, bool withKeyboard) final;
    void enterFullScreenForElement(WebCore::Element&) final;
    void exitFullScreenForElement(WebCore::Element*) final;
#endif

#if ENABLE(VIDEO)
    void setUpPlaybackControlsManager(WebCore::HTMLMediaElement&) override;
    void clearPlaybackControlsManager() override;

    void enterVideoFullscreenForVideoElement(WebCore::HTMLVideoElement&, WebCore::HTMLMediaElementEnums::VideoFullscreenMode, bool standby) override;
    void exitVideoFullscreenForVideoElement(WebCore::HTMLVideoElement&, WTF::CompletionHandler<void(bool)>&& completionHandler = [](bool) { }) override;
#endif

    bool supportsVideoFullscreen(WebCore::HTMLMediaElementEnums::VideoFullscreenMode) override;
    void wheelEventHandlersChanged(bool) final { }

    bool shouldUseTiledBackingForFrameView(const WebCore::FrameView&) const final;

    RefPtr<WebCore::Icon> createIconForFiles(const Vector<String>&) final;

    void didFinishLoadingImageForElement(WebCore::HTMLImageElement&) final;

    void requestCookieConsent(CompletionHandler<void(WebCore::CookieConsentDecisionResult)>&& completion) final;

    void classifyModalContainerControls(Vector<String>&&, CompletionHandler<void(Vector<WebCore::ModalContainerControlType>&&)>&&) final;

    void decidePolicyForModalContainer(OptionSet<WebCore::ModalContainerControlType>, CompletionHandler<void(WebCore::ModalContainerDecision)>&&) final;

private:
    WebPage& m_webPage;
};

}

