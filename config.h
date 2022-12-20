/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

#define PrintScreenDWM	0xff61

#define BOTTOM_GAP 0
#define TOP_GAP 0

/* appearance */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft =  0; 	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Sans Serif:size=10", "FontAwesome5Brands:size=10:antialias:true", "FontAwesome5Free:size=10:antialias:true", "FontAwesome5Free:style=Solid:size=10:antialias:true" };
static const char dmenufont[]       = "Sans Serif:size=10";
static const char col_gray1[]       = "#DCDAD5";
static const char col_gray2[]       = "#222222";
static const char col_gray3[]       = "#000000";
static const char col_gray4[]       = "#FFFFFF";
static const char col_gray5[]       = "#FF1E1E";
static const char col_cyan[]        = "#305397";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan },
};

/* tagging */
static const char *tags[] = { "main", "www", "irc", "media", "misc", "6", "7" };

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
//	{ "Gimp",     	 NULL,       NULL,       0,            True,        -1 },
	{ "firefox", NULL,       NULL,       1 << 1,       False,       -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "layouts/bstack.c"
#include "layouts/gaplessgrid.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=", tile },    /* first entry is default */
	{ "[F]", NULL },    /* no layout function means floating behavior */
	{ "[M]", monocle },
	{ "===", bstack },
	{ "HHH", gaplessgrid },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[] = { "xfce4-terminal", NULL };
static const char *vimcmd[] = { "xfce4-terminal", "-x", "vim", NULL };
static const char *pactlu[] = { "pactl", "set-sink-volume", "0", "+4%", NULL };
static const char *pactll[] = { "pactl", "set-sink-volume", "0", "-4%", NULL };
static const char *prtsc[] = {"scrot", NULL};

static Key keys[] = {
	/* modifier                     key        	  	 function       argument */
	{ MODKEY,			XK_F12,    	  	 spawn, 	{.v = termcmd } },
   	{ MODKEY,			XK_F7,     	  	 spawn, 	{.v = vimcmd } },
   	{ 0,     			XF86XK_AudioRaiseVolume, spawn, 	{.v = pactlu } },
	{ 0,     			XF86XK_AudioLowerVolume, spawn, 	{.v = pactll } },
   	{ MODKEY,               	XK_p,      		 spawn, 	{.v = dmenucmd } },
	{ 0, 				PrintScreenDWM,		 spawn, 	{.v = prtsc } },
	{ MODKEY,                       XK_b,      		 togglebar,     {0} },
	{ MODKEY,                       XK_j,      		 focusstack,	{.i = +1 } },
	{ MODKEY,                       XK_k,      		 focusstack,    {.i = -1 } },
	{ MODKEY,                       XK_i,      		 incnmaster,    {.i = +1 } },
	{ MODKEY,                       XK_d,      		 incnmaster,    {.i = -1 } },
	{ MODKEY,                       XK_h,      		 setmfact,      {.f = -0.05} },
	{ MODKEY,                       XK_l,      		 setmfact,      {.f = +0.05} },
	{ MODKEY,                       XK_Return, 		 zoom,          {0} },
	{ MODKEY,                       XK_Tab,    		 view,          {0} },
	{ MODKEY|ShiftMask,       	XK_c,      		 killclient,    {0} },
	{ MODKEY,                       XK_t,      		 setlayout,     {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      		 setlayout,     {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,    		 setlayout,     {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,    		XK_b,     		 setlayout,     {.v = &layouts[3]} }, /* bottomstack */
	{ MODKEY,     			XK_g,      		 setlayout,	{.v = &layouts[4]} }, /* grid */
	{ MODKEY,                       XK_space,  		 setlayout,     {0} },
	{ MODKEY|ShiftMask,       	XK_space,  		 togglefloating,{0} },
	{ MODKEY,                       XK_0,      		 view,          {.ui = ~0 } },
	{ MODKEY|ShiftMask,       	XK_0,      		 tag,           {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  		 focusmon,      {.i = -1 } },
	{ MODKEY,                       XK_period, 		 focusmon,      {.i = +1 } },
	{ MODKEY|ShiftMask,       	XK_comma,  		 tagmon,        {.i = -1 } },
	{ MODKEY|ShiftMask,       	XK_period, 		 tagmon,        {.i = +1 } },
	TAGKEYS(                        XK_1,                      		0)
	TAGKEYS(                        XK_2,                      		1)
	TAGKEYS(                        XK_3,                      		2)
	TAGKEYS(                        XK_4,                      		3)
	TAGKEYS(                        XK_5,                      		4)
	TAGKEYS(                        XK_6,                      		5)
	TAGKEYS(                        XK_7,                      		6)
	TAGKEYS(                        XK_8,                      		7)
	TAGKEYS(                        XK_9,                      		8)
	{ MODKEY|ShiftMask,             XK_q,      		 quit,		{0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

