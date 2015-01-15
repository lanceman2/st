#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../libst/st_debug.h"
#include "stg_debug.h"
#include <glib.h>


static
void logHandler(const gchar *log_domain,
          GLogLevelFlags log_level,
          const gchar *message,
          gpointer user_data)
{
  if(log_level == G_LOG_LEVEL_WARNING)
  {
    const char *G_BUG_MESSAGE =
      "Couldn't register with accessibility bus: "
      "Did not receive a reply. Possible causes i"
      "nclude: t";
    if(!strncmp(G_BUG_MESSAGE, message, strlen(G_BUG_MESSAGE)))
    {
      fprintf(stderr, "We caught error: "
      "log_domain=\"%s\" log_level=%d message=\"%s\"",
      log_domain, log_level, message);
      // Keep running if this is from this known GTK 3 bug.
      fprintf(stderr, "This is from a known bug in GTK 3\n");
      return;
    }
  }

  ST_VASSERT(0, "We caught error: "
      "log_domain=\"%s\" log_level=%d message=\"%s\"",
      log_domain, log_level, message);
}

void stg_init(void)
{
  static __thread bool init = false;
  if(init) return;
  init = true;
  st_init(); // stDebug_init() is re-entrance safe
  g_log_set_handler(NULL,
      G_LOG_LEVEL_WARNING | G_LOG_FLAG_FATAL |
      G_LOG_LEVEL_ERROR | G_LOG_LEVEL_CRITICAL |
      G_LOG_FLAG_RECURSION, logHandler, NULL);
  g_log_set_handler("Gtk",
      G_LOG_LEVEL_WARNING | G_LOG_FLAG_FATAL |
      G_LOG_LEVEL_ERROR | G_LOG_LEVEL_CRITICAL |
      G_LOG_FLAG_RECURSION, logHandler, NULL);
  g_log_set_handler("GLib",
      G_LOG_LEVEL_WARNING | G_LOG_FLAG_FATAL |
      G_LOG_LEVEL_ERROR | G_LOG_LEVEL_CRITICAL |
      G_LOG_FLAG_RECURSION, logHandler, NULL);
  // Just guessing that there is a handler for the Gdk API too.
  g_log_set_handler("Gdk",
      G_LOG_LEVEL_WARNING | G_LOG_FLAG_FATAL |
      G_LOG_LEVEL_ERROR | G_LOG_LEVEL_CRITICAL |
      G_LOG_FLAG_RECURSION, logHandler, NULL);
}
