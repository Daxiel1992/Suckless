/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
//static const char *fonts[]          = { "Liberation Mono:size=11", "fontawesome:size=10"};
//static const char dmenufont[]       = "Liberation Mono:size=11";
static const char *fonts[] = { "JetBrainsMono Nerd Font:size=10" };
static const char dmenufont[] = "JetBrainsMono Nerd Font:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";

/*
static const char s_base03[]        = "#002b36";
static const char s_base02[]        = "#073642";
static const char s_base01[]        = "#586e75";
static const char s_base00[]        = "#657b83";
static const char s_base0[]         = "#839496";
static const char s_base1[]         = "#93a1a1";
static const char s_base2[]         = "#eee8d5";
static const char s_base3[]         = "#fdf6e3";
*/
						    //
static const char col_gbg[]        = "#000000"; // inactive background
static const char col_gfg[]        = "#a4d1c7"; // inactive text
static const char col_gborder[]    = "#3e7f68"; // inactive border
static const char col_gselbg[]     = "#71A898"; // active background
static const char col_gselfg[]     = "#000000"; // active text
static const char col_gselborder[] = "#a4d1c7"; // active border

static const char col_obg[]        = "#232020"; // inactive background
static const char col_ofg[]        = "#F4F4F4"; // inactive text
static const char col_oborder[]    = "#3A3535"; // inactive border
static const char col_oselbg[]     = "#FF7315"; // active background
static const char col_oselfg[]     = "#3A3535"; // active text
static const char col_oselborder[] = "#F4F4F4"; // active border

static const char col_cpbg[]        = "#01012b"; // inactive background
static const char col_cpfg[]        = "#05d9e8"; // inactive text
static const char col_cpborder[]    = "#005678"; // inactive border
static const char col_cpselbg[]     = "#ff2a6d"; // active background
static const char col_cpselfg[]     = "#d1f7ff"; // active text
static const char col_cpselborder[] = "#ff2a6d"; // active border
						//
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	/*{ s_base0, s_base03, s_base2 },       SchemeNorm dark */
	/*{ s_base0, s_base02, s_base2 },       SchemeSel dark */
	/*{ s_base00, s_base3, s_base02 },      SchemeNorm light */
	/*{ s_base00, s_base2, s_base02},       SchemeSel light */ 
	{ col_cpfg, col_cpbg, col_cpborder }, /* SchemeNorm CyberPunk*/
	{ col_cpselfg, col_cpselbg, col_cpselborder }, /* SchemeSel CyberPunk */
	{ col_gfg, col_gbg, col_gborder }, /* SchemeNorm Green */
	{ col_gselfg, col_gselbg, col_gselborder }, /* SchemeSel Green */
	{ col_ofg, col_obg, col_oborder }, /* SchemeNorm Green */
	{ col_oselfg, col_oselbg, col_oselborder }, /* SchemeSel Green */
	{ col_gray3, col_gray1, col_gray2 }, /* SchemeNorm orig */
	{ col_gray4, col_cyan,  col_cyan  }, /* SchemeSel orig */
};

/* tagging */
static const char *tags[] = { "󰌀", "", "󰨞", "", "", "", "󰄮"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Brave-browser", NULL,  NULL,       1 << 0,       0,           -1 },
	{ "code-oss", NULL,       NULL,       1 << 2,       0,           -1 },
	{ "libreoffice-startcenter", NULL, NULL, 1 << 3,    0,           -1 },
	{ "Spotify",  NULL, 	  NULL,       1 << 4,	    0,           -1 },
	{ "steam",    NULL,	  NULL,       1 << 5,	    1,		 -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, "-l", "30", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *browser[] = { "brave", NULL};
static const char *logout[] = { "/bin/bash", "-c", "pkill -u dan", NULL};
static const char *shutdown[] = { "/bin/bash", "-c", "sudo systemctl poweroff", NULL};
static const char *reboot[] = { "/bin/bash", "-c", "sudo systemctl reboot", NULL};
static const char *volup[] = { "/bin/bash", "-c", "amixer -D default sset Master Playback 5%+", "&&", "/home/dan/bin/sv", NULL};
static const char *voldown[] = { "/bin/bash", "-c", "amixer -D default sset Master Playback 5%-", "&&", "/home/dan/bin/sv", NULL};
//static const char *volmute[] = { "/bin/bash", "-c", "amixer -D default sset Master Playback 0%", NULL};
static const char *pauseplay[] = { "/bin/bash", "-c", "playerctl play-pause", NULL};
static const char *audionext[] = { "/bin/bash", "-c", "playerctl next", NULL};
static const char *audioprev[] = { "/bin/bash", "-c", "playerctl previous", NULL};
static const char *brightup[] = { "/bin/bash", "-c", "backlight_control +10", NULL};
static const char *brightdown[] = { "/bin/bash", "-c", "backlight_control -10", NULL};
static const char *screenshot[] = { "/bin/bash", "-c", "shotgun", NULL};


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawndmenu,     {0} },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,			XK_w,	   spawn,	   {.v = browser} },
	{ MODKEY,			XK_F1,	   quit,	   {0} },
	{ MODKEY,			XK_F2,	   spawn,	   {.v = shutdown} },
	{ MODKEY,			XK_F3,	   spawn,	   {.v = reboot} },
	{ MODKEY,			XK_F5,	   spawn,	   {.v = brightdown} },
	{ MODKEY,			XK_F6,	   spawn,	   {.v = brightup} },
	{ MODKEY,			XK_minus,  spawn,          {.v = voldown} },
	{ MODKEY,			XK_equal,  spawn,          {.v = volup} },
	{ MODKEY,			XK_End,	   spawn,   	   {.v = pauseplay} },
	{ MODKEY,		 XK_Page_Down,     spawn,	   {.v = audionext} },
	{ MODKEY,		XK_Page_Up,       spawn,	   {.v = audioprev} },
	{ MODKEY,		XF86XK_AudioPlay,    spawn,	   {.v = pauseplay} },
	{ MODKEY|ShiftMask,             XK_s, 	   spawn,          {.v = screenshot } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_t,      schemeToggle,   {0} },
	{ MODKEY|ShiftMask,             XK_z,      schemeCycle,    {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask,           XK_q,    quit,             {.i = 23} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

