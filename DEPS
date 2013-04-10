# Do NOT add chrome to the list below.  We shouldn't be including files from
# src/chrome in src/content.
include_rules = [
  # The subdirectories in content/ will manually allow their own include
  # directories in content/ so we disallow all of them.
  "-content",
  "+content/common",
  "+content/port/common",
  "+content/public/common",
  "+content/public/test",
  "+content/shell",  # for content_browsertests
  "+content/test",

  # content isn't tied to prefs so that other embedders are able to pick
  # different ways of storing their preferences. Also, this is to avoid prefs
  # being used as a parallel API to the Content API.
  "-base/prefs",

  "+cc",
  "+crypto",
  "+grit/content_resources.h",
  "+grit/ui_resources.h",
  "+grit/webkit_chromium_resources.h",
  "+grit/webkit_resources.h",
  "+grit/webkit_strings.h",
  "+grit/webui_resources_map.h",

  "+dbus",
  "+gpu",
  "+net",
  "+ppapi",
  "+printing",
  "+sandbox",
  "+skia",

  # In general, content/ should not rely on google_apis, since URLs
  # and access tokens should usually be provided by the
  # embedder.
  #
  # There are a couple of specific parts of content that are excepted
  # from this rule, see content/browser/speech/DEPS and
  # content/browser/geolocation/DEPS.  Both of these are cases of
  # implementations that are strongly tied to Google servers, i.e. we
  # don't expect alternate implementations to be provided by the
  # embedder.
  "-google_apis",

  # Don't allow inclusion of these other libs we shouldn't be calling directly.
  "-v8",
  "-tools",

  # Allow inclusion of third-party code:
  "+third_party/angle",
  "+third_party/flac",
  "+third_party/gpsd",
  "+third_party/mozilla",
  "+third_party/npapi/bindings",
  "+third_party/skia",
  "+third_party/sqlite",
  "+third_party/tcmalloc",
  "+third_party/khronos",
  "+third_party/webrtc",
  "+third_party/WebKit/Source/Platform/chromium",
  "+third_party/WebKit/Source/WebKit/chromium",

  "+ui/android",
  # Aura is analogous to Win32 or a Gtk, so it is allowed.
  "+ui/aura",
  "+ui/base",
  "+ui/compositor",
  "+ui/gfx",
  "+ui/gl",
  "+ui/native_theme",
  "+ui/shell_dialogs",
  "+ui/snapshot",
  "+ui/surface",
  # Content knows about grd files, but the specifics of how to get a resource
  # given its id is left to the embedder.
  "-ui/base/l10n",
  "-ui/base/resource",
  # This file isn't related to grd, so it's fine.
  "+ui/base/l10n/l10n_util_win.h",

  # Content shouldn't depend on views. While we technically don't need this
  # line, since the top level DEPS doesn't allow it, we add it to make this
  # explicit.
  "-ui/views",

  "+webkit",
  "-webkit/dom_storage",
  "+webkit/dom_storage/dom_storage_types.h",

  # For generated JNI includes.
  "+jni",
]
