// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ANDROID_CONTENT_VIEW_CORE_IMPL_H_
#define CONTENT_BROWSER_ANDROID_CONTENT_VIEW_CORE_IMPL_H_

#include <vector>

#include "base/android/jni_helper.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/i18n/rtl.h"
#include "base/memory/scoped_ptr.h"
#include "base/process.h"
#include "content/browser/renderer_host/render_widget_host_view_android.h"
#include "content/browser/web_contents/web_contents_impl.h"
#include "content/public/browser/android/content_view_core.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "googleurl/src/gurl.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebInputEvent.h"
#include "ui/gfx/rect.h"

struct WebMenuItem;

namespace ui {
class WindowAndroid;
}

namespace content {
class RenderWidgetHostViewAndroid;
class SyncInputEventFilter;

// TODO(jrg): this is a shell.  Upstream the rest.
class ContentViewCoreImpl : public ContentViewCore,
                            public NotificationObserver {
 public:
  static ContentViewCoreImpl* FromWebContents(WebContents* web_contents);
  ContentViewCoreImpl(JNIEnv* env,
                      jobject obj,
                      bool hardware_accelerated,
                      bool input_events_delivered_at_vsync,
                      WebContents* web_contents,
                      ui::WindowAndroid* window_android);

  // ContentViewCore implementation.
  virtual base::android::ScopedJavaLocalRef<jobject> GetJavaObject() OVERRIDE;
  virtual base::android::ScopedJavaLocalRef<jobject> GetContainerViewDelegate()
      OVERRIDE;
  virtual WebContents* GetWebContents() const OVERRIDE;
  virtual ui::WindowAndroid* GetWindowAndroid() const OVERRIDE;
  virtual scoped_refptr<cc::Layer> GetLayer() const OVERRIDE;
  virtual void LoadUrl(NavigationController::LoadURLParams& params) OVERRIDE;
  virtual void OnWebPreferencesUpdated() OVERRIDE;
  virtual jint GetCurrentRenderProcessId(JNIEnv* env, jobject obj) OVERRIDE;
  virtual void ShowPastePopup(int x, int y) OVERRIDE;
  virtual unsigned int GetScaledContentTexture(
      float scale,
      gfx::Size* out_size) OVERRIDE;
  virtual float GetDpiScale() const OVERRIDE;
  virtual void SetInputHandler(
      WebKit::WebCompositorInputHandler* input_handler) OVERRIDE;
  virtual void AddFrameInfoCallback(
      const UpdateFrameInfoCallback& callback) OVERRIDE;
  virtual void RemoveFrameInfoCallback(
      const UpdateFrameInfoCallback& callback) OVERRIDE;

  // --------------------------------------------------------------------------
  // Methods called from Java via JNI
  // --------------------------------------------------------------------------

  void OnJavaContentViewCoreDestroyed(JNIEnv* env, jobject obj);

  // Notifies the ContentViewCore that items were selected in the currently
  // showing select popup.
  void SelectPopupMenuItems(JNIEnv* env, jobject obj, jintArray indices);

  void LoadUrl(
      JNIEnv* env, jobject obj,
      jstring url,
      jint load_url_type,
      jint transition_type,
      jint ua_override_option,
      jstring extra_headers,
      jbyteArray post_data,
      jstring base_url_for_data_url,
      jstring virtual_url_for_data_url,
      jboolean can_load_local_resources);
  void SetAllUserAgentOverridesInHistory(
      JNIEnv* env,
      jobject obj,
      jstring user_agent_override);
  base::android::ScopedJavaLocalRef<jstring> GetURL(JNIEnv* env, jobject) const;
  base::android::ScopedJavaLocalRef<jstring> GetTitle(
      JNIEnv* env, jobject obj) const;
  jboolean IsIncognito(JNIEnv* env, jobject obj);
  jboolean Crashed(JNIEnv* env, jobject obj) const { return tab_crashed_; }
  void SendOrientationChangeEvent(JNIEnv* env, jobject obj, jint orientation);
  jboolean SendTouchEvent(JNIEnv* env,
                          jobject obj,
                          jlong time_ms,
                          jint type,
                          jobjectArray pts);
  jboolean SendMouseMoveEvent(JNIEnv* env,
                              jobject obj,
                              jlong time_ms,
                              jfloat x,
                              jfloat y);
  jboolean SendMouseWheelEvent(JNIEnv* env,
                               jobject obj,
                               jlong time_ms,
                               jfloat x,
                               jfloat y,
                               jfloat vertical_axis);
  void ScrollBegin(JNIEnv* env, jobject obj, jlong time_ms, jfloat x, jfloat y);
  void ScrollEnd(JNIEnv* env, jobject obj, jlong time_ms);
  void ScrollBy(JNIEnv* env, jobject obj, jlong time_ms,
                jfloat x, jfloat y, jfloat dx, jfloat dy);
  void FlingStart(JNIEnv* env, jobject obj, jlong time_ms,
                  jfloat x, jfloat y, jfloat vx, jfloat vy);
  void FlingCancel(JNIEnv* env, jobject obj, jlong time_ms);
  void SingleTap(JNIEnv* env, jobject obj, jlong time_ms,
                 jfloat x, jfloat y,
                 jboolean disambiguation_popup_tap);
  void ShowPressState(JNIEnv* env, jobject obj, jlong time_ms,
                      jfloat x, jfloat y);
  void ShowPressCancel(JNIEnv* env, jobject obj, jlong time_ms,
                       jfloat x, jfloat y);
  void DoubleTap(JNIEnv* env, jobject obj, jlong time_ms,
                 jfloat x, jfloat y) ;
  void LongPress(JNIEnv* env, jobject obj, jlong time_ms,
                 jfloat x, jfloat y,
                 jboolean disambiguation_popup_tap);
  void LongTap(JNIEnv* env, jobject obj, jlong time_ms,
               jfloat x, jfloat y,
               jboolean disambiguation_popup_tap);
  void PinchBegin(JNIEnv* env, jobject obj, jlong time_ms, jfloat x, jfloat y);
  void PinchEnd(JNIEnv* env, jobject obj, jlong time_ms);
  void PinchBy(JNIEnv* env, jobject obj, jlong time_ms,
               jfloat x, jfloat y,
               jfloat delta);
  void SelectBetweenCoordinates(JNIEnv* env, jobject obj,
                                jfloat x1, jfloat y1,
                                jfloat x2, jfloat y2);
  void MoveCaret(JNIEnv* env, jobject obj, jfloat x, jfloat y);

  jboolean CanGoBack(JNIEnv* env, jobject obj);
  jboolean CanGoForward(JNIEnv* env, jobject obj);
  jboolean CanGoToOffset(JNIEnv* env, jobject obj, jint offset);
  void GoBack(JNIEnv* env, jobject obj);
  void GoForward(JNIEnv* env, jobject obj);
  void GoToOffset(JNIEnv* env, jobject obj, jint offset);
  void GoToNavigationIndex(JNIEnv* env, jobject obj, jint index);
  void StopLoading(JNIEnv* env, jobject obj);
  void Reload(JNIEnv* env, jobject obj);
  void CancelPendingReload(JNIEnv* env, jobject obj);
  void ContinuePendingReload(JNIEnv* env, jobject obj);
  jboolean NeedsReload(JNIEnv* env, jobject obj);
  void ClearHistory(JNIEnv* env, jobject obj);
  void EvaluateJavaScript(JNIEnv* env,
                          jobject obj,
                          jstring script,
                          jobject callback);
  int GetNativeImeAdapter(JNIEnv* env, jobject obj);
  void SetFocus(JNIEnv* env, jobject obj, jboolean focused);
  void ScrollFocusedEditableNodeIntoView(JNIEnv* env, jobject obj);
  void UndoScrollFocusedEditableNodeIntoView(JNIEnv* env, jobject obj);

  jint GetBackgroundColor(JNIEnv* env, jobject obj);
  void SetBackgroundColor(JNIEnv* env, jobject obj, jint color);
  void OnShow(JNIEnv* env, jobject obj);
  void OnHide(JNIEnv* env, jobject obj);
  void ClearSslPreferences(JNIEnv* env, jobject /* obj */);
  void SetUseDesktopUserAgent(JNIEnv* env,
                              jobject /* obj */,
                              jboolean state,
                              jboolean reload_on_state_change);
  bool GetUseDesktopUserAgent(JNIEnv* env, jobject /* obj */);
  void Show();
  void Hide();
  void AddJavascriptInterface(JNIEnv* env,
                              jobject obj,
                              jobject object,
                              jstring name,
                              jclass safe_annotation_clazz,
                              jobject retained_object_set);
  void RemoveJavascriptInterface(JNIEnv* env, jobject obj, jstring name);
  int GetNavigationHistory(JNIEnv* env, jobject obj, jobject context);
  void GetDirectedNavigationHistory(JNIEnv* env,
                                    jobject obj,
                                    jobject context,
                                    jboolean is_forward,
                                    jint max_entries);
  void UpdateVSyncParameters(JNIEnv* env, jobject obj, jlong timebase_micros,
                             jlong interval_micros);
  jboolean PopulateBitmapFromCompositor(JNIEnv* env,
                                        jobject obj,
                                        jobject jbitmap);
  void WasResized(JNIEnv* env, jobject obj);
  jboolean IsRenderWidgetHostViewReady(JNIEnv* env, jobject obj);
  void ExitFullscreen(JNIEnv* env, jobject obj);
  void EnableHidingTopControls(JNIEnv* env, jobject obj, bool enable);
  void ShowImeIfNeeded(JNIEnv* env, jobject obj);

  void ShowInterstitialPage(JNIEnv* env,
                            jobject obj,
                            jstring jurl,
                            jint delegate);
  jboolean IsShowingInterstitialPage(JNIEnv* env, jobject obj);

  jboolean ConsumePendingRendererFrame(JNIEnv* env, jobject obj);

  void AttachExternalVideoSurface(JNIEnv* env,
                                  jobject obj,
                                  jint player_id,
                                  jobject jsurface);
  void DetachExternalVideoSurface(JNIEnv* env, jobject obj, jint player_id);

  // --------------------------------------------------------------------------
  // Public methods that call to Java via JNI
  // --------------------------------------------------------------------------

  // Creates a popup menu with |items|.
  // |multiple| defines if it should support multi-select.
  // If not |multiple|, |selected_item| sets the initially selected item.
  // Otherwise, item's "checked" flag selects it.
  void ShowSelectPopupMenu(const std::vector<WebMenuItem>& items,
                           int selected_item,
                           bool multiple);

  void OnTabCrashed();

  // All sizes and offsets are in CSS pixels as cached by the renderer.
  void UpdateFrameInfo(const gfx::Vector2dF& scroll_offset,
                       float page_scale_factor,
                       const gfx::Vector2dF& page_scale_factor_limits,
                       const gfx::SizeF& content_size,
                       const gfx::SizeF& viewport_size,
                       const gfx::Vector2dF& controls_offset,
                       const gfx::Vector2dF& content_offset,
                       float overdraw_bottom_height);

  void UpdateImeAdapter(int native_ime_adapter, int text_input_type,
                        const std::string& text,
                        int selection_start, int selection_end,
                        int composition_start, int composition_end,
                        bool show_ime_if_needed);
  void ProcessImeBatchStateAck(bool is_begin);
  void SetTitle(const string16& title);
  void OnBackgroundColorChanged(SkColor color);

  bool HasFocus();
  void ConfirmTouchEvent(InputEventAckState ack_result);
  void HasTouchEventHandlers(bool need_touch_events);
  void OnSelectionChanged(const std::string& text);
  void OnSelectionBoundsChanged(
      const ViewHostMsg_SelectionBounds_Params& params);

  void StartContentIntent(const GURL& content_url);

  // Shows the disambiguation popup
  // |target_rect|   --> window coordinates which |zoomed_bitmap| represents
  // |zoomed_bitmap| --> magnified image of potential touch targets
  void ShowDisambiguationPopup(
      const gfx::Rect& target_rect, const SkBitmap& zoomed_bitmap);

  void RequestExternalVideoSurface(int player_id);

  // --------------------------------------------------------------------------
  // Methods called from native code
  // --------------------------------------------------------------------------

  gfx::Size GetPhysicalBackingSize() const;
  gfx::Size GetViewportSizeDip() const;
  float GetOverdrawBottomHeightDip() const;

  InputEventAckState FilterInputEvent(const WebKit::WebInputEvent& input_event);

  void AttachLayer(scoped_refptr<cc::Layer> layer);
  void RemoveLayer(scoped_refptr<cc::Layer> layer);
  void DidProduceRendererFrame();

 private:
  class ContentViewUserData;

  friend class ContentViewUserData;
  virtual ~ContentViewCoreImpl();

  // NotificationObserver implementation.
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  // --------------------------------------------------------------------------
  // Other private methods and data
  // --------------------------------------------------------------------------

  void InitWebContents();

  RenderWidgetHostViewAndroid* GetRenderWidgetHostViewAndroid();

  float GetTouchPaddingDip();

  WebKit::WebGestureEvent MakeGestureEvent(
      WebKit::WebInputEvent::Type type, long time_ms,
      float xPix, float yPix) const;

  gfx::Size GetViewportSizePix() const;

  void UpdateVSyncFlagOnInputEvent(WebKit::WebInputEvent* event) const;

  void DeleteScaledSnapshotTexture();

  void SendGestureEvent(const WebKit::WebGestureEvent& event);

  // A weak reference to the Java ContentViewCore object.
  JavaObjectWeakGlobalRef java_ref_;

  NotificationRegistrar notification_registrar_;

  // Reference to the current WebContents used to determine how and what to
  // display in the ContentViewCore.
  WebContentsImpl* web_contents_;

  // A compositor layer containing any layer that should be shown.
  scoped_refptr<cc::Layer> root_layer_;

  // Whether the renderer backing this ContentViewCore has crashed.
  bool tab_crashed_;

  // Whether input events will be consistently delivered at vsync time.
  bool input_events_delivered_at_vsync_;

  // Whether a new frame from the renderer is waiting to be displayed by the
  // browser compositor.
  bool renderer_frame_pending_;

  // Device scale factor.
  float dpi_scale_;

  // The owning window that has a hold of main application activity.
  ui::WindowAndroid* window_android_;

  std::vector<UpdateFrameInfoCallback> update_frame_info_callbacks_;

  // Optional browser-side input event filtering.
  scoped_ptr<SyncInputEventFilter> input_event_filter_;

  DISALLOW_COPY_AND_ASSIGN(ContentViewCoreImpl);
};

bool RegisterContentViewCore(JNIEnv* env);

}  // namespace content

#endif  // CONTENT_BROWSER_ANDROID_CONTENT_VIEW_CORE_IMPL_H_
