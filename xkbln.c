#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

char cmd_buf[512];

int is_special_name(char* name)
{
  char* sn[] = { "pc", "group", "inet", "compose", "terminate", "level", NULL };
  int i;
  for(i = 0; sn[i] != NULL; ++i)
  {
    if (strncmp(name, sn[i], strlen(sn[i])) == 0)
      return 1;
  }
  return 0;
}

char* find_symbol(char* symbols, int group)
{
  int i = 0;
  char* cur = symbols;
  while(cur)
  {
    if (!is_special_name(cur))
    {
      if (group == i)
      {
        char* end = strchr(cur, '+');
        if (end)
          *end = '\0';
        end = strchr(cur, ':');
        if (end)
          *end = '\0';
        return cur;
      }
      ++i;
    }
    cur = strchr(cur, '+');
    if (cur)
      ++cur;
  }
  return cur;
}

Atom get_symbols_atom(Display* dpy)
{
  Atom result = 0;
  XkbDescRec desc;
  desc.device_spec = XkbUseCoreKbd;
  desc.names = NULL;
  if (XkbGetNames(dpy, XkbSymbolsNameMask, &desc) == Success)
  {
    result = desc.names->symbols;
    XkbFreeNames(&desc, XkbSymbolsNameMask, 0);
  }
  return result;
}

void handle_group_change(Display* dpy, int group, char* cmd)
{
  char* symbols = XGetAtomName(dpy, get_symbols_atom(dpy));
  if (symbols)
  {
    char* sym = find_symbol(symbols, group);
    if (sym)
    {
      if (cmd)
      {
        sprintf(cmd_buf, cmd, sym);
        system(cmd_buf);
      }
      else
      {
        printf("%s\n", sym);
      }
    }
    XFree(symbols);
  }
}

int main(int argc, char** argv)
{
  int xkbEventType, xkbError, mjr, mnr, reason_returned;
	mjr = XkbMajorVersion;
	mnr = XkbMinorVersion;
	Display* dpy = XkbOpenDisplay(NULL, 
      &xkbEventType, &xkbError, &mjr, &mnr, &reason_returned);
	if (dpy == NULL)
  {
    printf("Cannot open display!\n");
    return 1;
  }
	XkbSelectEventDetails(dpy, XkbUseCoreKbd, XkbStateNotify,
						  XkbAllStateComponentsMask, XkbGroupStateMask);

  XkbStateRec state;
  XkbGetState(dpy, XkbUseCoreKbd, &state);
  if (argc < 2)
  {
    handle_group_change(dpy, (int)state.locked_group, NULL);
    return 0;
  }
  handle_group_change(dpy, (int)state.locked_group, argv[1]);
  
  while(1)
  {
	  XkbEvent ev;
		XNextEvent(dpy, &ev.core);
    if (ev.type == xkbEventType && ev.any.xkb_type == XkbStateNotify)
      handle_group_change(dpy, ev.state.locked_group, argv[1]);
  }
  return 0;
}
