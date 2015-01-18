#include <limits.h>
#include <sys/wait.h>
#include <libgen.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <gtk/gtk.h>

struct Run
{
  int run_count;
  GtkWidget *button;
  char *program;
};


// Add the Path of this program to the environment
// variable PATH
static void setPathEnv(void)
{
  // TODO: /proc/self/exe is specific to Linux
  //
  char *path = realpath("/proc/self/exe" , NULL);
  if(!path) return;
  if(!path[0]) goto cleanup;

  // Remove filename and last '/'
  char *end;
  end = path;
  while(*(++end));
  --end;
  while(*end != '/' && end != path)
    *(end--) = '\0';
  if(end != path && *end == '/')
    *end = '\0';

  char *envPath;
  envPath = getenv("PATH");
  if(!envPath) goto cleanup;

  char *newPath;
  newPath = g_strdup_printf("%s:%s", path, envPath);

  setenv("PATH", newPath, 1);
  //fprintf(stderr, "set PATH=\"%s\"\n", newPath);

  g_free(newPath);

cleanup:

  free(path);
}

static
void setLabelString(struct Run *run)
{
  char *text = NULL;
  GtkLabel *label;
  label = GTK_LABEL(gtk_bin_get_child(GTK_BIN(run->button)));
  char *color;
  switch(run->run_count)
  {
    case 0:
      color = "#000";
      break;
    default:
      color = "#f55";
      break;
  }

  text = g_markup_printf_escaped("%s", run->program);

  // We make all the labels the same length
  // by padding with white space.
  char *old;
  old = text;
  text = g_strdup_printf(
          "<span font_family=\"monospace\" "
          ">%4d)  </span>"
          "<span font_family=\"monospace\" "
          "font_weight=\"bold\" "
          "color=\"%s\">%s %s",
          run->run_count++, color, old,
          "                                                 "
          "                                                 "
          "                                                 ");
  g_free(old);
  old = text;
  text = g_strdup_printf(
          "%168.168s</span>", old);
  g_free(old);

  gtk_label_set_markup(label, text);
  g_free(text);
}

static bool run_cb(GtkWidget *button, struct Run *run)
{
  fprintf(stderr, "running:");
  fprintf(stderr, " %s", run->program);
  fprintf(stderr, "\n");

  setLabelString(run);

  // pipe command to bash

  int fd[2];

  if(pipe(fd) == -1)
  {
    fprintf(stderr, "pipe() failed: %s\n",
        strerror(errno));
    return true;
  }

  if(fork() == 0)
  {
    // child
    close(fd[1]);
    // make stdin be the reading end of the pipe
    if(dup2(fd[0], 0) == -1)
    {
      fprintf(stderr, "dup2(%d, 0) failed: %s\n",
          fd[0], strerror(errno));
      exit(1);
    }
    // bash will read the pipe
    execl("/bin/bash", "bash", NULL);
    fprintf(stderr, "failed to execute: %s in bash pipeline\n", run->program);
    exit(1);
  }

  // The parent write the bash command to the pipe
  if(write(fd[1], run->program, strlen(run->program)) != strlen(run->program))
  {
    fprintf(stderr, "write(%d, \"%s\", %zu) failed: %s\n",
        fd[1], run->program, strlen(run->program), strerror(errno));
    exit(1);
  }
  if(write(fd[1], "\n", 2) != 2)
  {
    fprintf(stderr, "write(%d, \"%s\", %d) failed: %s\n",
        fd[1], "\\n", 2, strerror(errno));
    exit(1);
  }
  close(fd[0]);
  close(fd[1]);
  return true;
}

static void addButton(const char *args, GtkContainer *vbox)
{
  struct Run *run;
  run = g_malloc0(sizeof(*run));
  run->button = gtk_button_new_with_label(" ");
  run->program = (char *) args;
  g_signal_connect(run->button, "clicked", G_CALLBACK(run_cb), (void *) run);
  gtk_box_pack_start(GTK_BOX(vbox), run->button, false, false, 0);
  gtk_widget_show(run->button);
  setLabelString(run);
}

static
bool ecb_keyPress(GtkWidget *w, GdkEvent *e, void* data)
{
  switch(e->key.keyval)
  {
    case GDK_KEY_Q:
    case GDK_KEY_q:
    case GDK_KEY_Escape:
      gtk_main_quit();
      return true;
      break;
  }
  return false;
}

static void makeWidgets(const char *title, char **programs)
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *button;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), title);
  g_signal_connect(G_OBJECT(window), "key-press-event",
      G_CALLBACK(ecb_keyPress), NULL);
  g_signal_connect(window, "delete_event", G_CALLBACK(gtk_main_quit), NULL);

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  button = gtk_button_new_with_label("Quit");
  g_signal_connect(button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), button, false, false, 0);
  gtk_widget_show(button);
  
  gtk_container_add(GTK_CONTAINER(window), vbox);

  while(*programs)
    addButton(*programs++, GTK_CONTAINER(vbox));

  gtk_widget_show(vbox);

  gtk_widget_show(window);
}

static int
usage(char *argv0)
{
    fprintf(stderr,
        "Usage: %s LIST ... [--title TITLE]|[--help|-h]\n\n"
        "   A simple button launcher program.  This launcher does\n"
        " not manage the programs that it launches, so quiting this\n"
        " launcher will not terminate all the launched programs.  If\n"
        " you terminate this launcher by signaling it from a shell,\n"
        " the shell may also terminate all the launched programs too,\n"
        " but that's just the shell doing its thing and not this\n"
        " launcher.  This launcher program is intentionally simple and\n"
        " is not a graphical shell.  It's just a program launcher.\n"
        "\n"
        "\n"
        "        OPTIONS\n"
        "\n"
        "  --help|-h      print this help and exit\n"
        "\n"
        "  LIST           read a list of programs to run from the file LIST\n"
        "                 one on each line in the file.  The programs are run\n"
        "                 using /bin/bash.  If LIST is '-' stdin will be read\n"
        "\n"
        "  --title TITLE  set window title to TITLE\n"
        "\n"
        ,
        basename(argv0));
    return 1;
}

static
int getFileList(char *filename, char ***programs, int *num_progs)
{
  FILE *file;
  if(filename[0] == '-' && filename[1] == '\0')
    file = stdin;
  else if(!(file = fopen(filename, "r")))
  {
    fprintf(stderr, "fopen(\"%s\", 'r') failed: %s\n\n",
        filename, strerror(errno));
    return 1; // fail
  }

  int n = 0;
  char *line = NULL;
  size_t line_length = 0;
  while(getline(&line, &line_length, file) != -1)
  {
    if(line && line[0] && line[0] != '#')
    {
      size_t l;
      while((l = strlen(line)) && (line[strlen(line) - 1] == '\n' ||
        line[strlen(line) - 1] == '\r'))
        line[strlen(line) - 1] = '\0';
      if(line[0])
      {
        ++n;
        *programs = realloc(*programs, sizeof(char **) * (++(*num_progs) + 1));
        (*programs)[*num_progs - 1] = strdup(line);
        (*programs)[*num_progs] = NULL;
      }
    }
  }
  if(line)
    free(line);

  if(n == 0)
    fprintf(stderr,
        "No programs listed in file: %s\n",
        filename);

  if(filename[0] != '-' || filename[1] != '\0')
    fclose(file);

  return 0; // success
}

static
void sigChildCatcher(int sig)
{
  int status;
  wait(&status);
}

int main(int argc, char **argv)
{
  char **programs = NULL;
  char *lists = NULL;
  int num_progs = 0;
  char **args;
  args = argv;
  char *title = NULL;

  while(*(++args))
  {
    if(strncmp(*args, "--title=", 8) == 0)
      title = &(*args)[8];
    else if(strcmp(*args, "--title") == 0)
      title = *(++args);
    else if(strcmp(*args, "--help") == 0 ||
        strcmp(*args, "-h") == 0)
      return usage(argv[0]);
    else
    {
      if(getFileList(*args, &programs, &num_progs))
        return usage(argv[0]);
      
      if(lists)
      {
        char *s;
        s = lists;
        lists = g_strjoin(" ", lists, *args, NULL);
        g_free(s);
      }
      else
        lists = g_strdup(*args);
    }
  }
  if(num_progs == 0)
    return usage(argv[0]);

  if(!title)
    title = g_strdup_printf("%s %s", basename(argv[0]), lists);
  else
    title = g_strdup(title);
  g_free(lists);

  setPathEnv();
  gtk_init(&argc, &argv);
  makeWidgets(title, programs);
  g_free(title);
  signal(SIGCHLD, sigChildCatcher);

  gtk_main();

  return 0;
}
