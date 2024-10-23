/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
#include "termcolors.h"

/* appearance */
static const unsigned int borderpx	= 2;
static const unsigned int snap		= 32;
static const unsigned int gappih	= 10;
static const unsigned int gappiv	= 10;
static const unsigned int gappoh	= 10;
static const unsigned int gappov	= 10;
static	int smartgaps				= 0;
static const int showbar			= 1;
static const int topbar				= 1;
static const int user_bh			= 8;
static const int vertpad			= 10;
static const int sidepad			= 10;
static const char buttonbar[]		= " ";
#define ICONSIZE					(bh - 12)
#define ICONSPACING					10 
static const char *fonts[]			= { "Aptos:style=SemiBold:size=15",
										"JetBrainsMono Nerd Font:style=ExtraBold:size=13",
										"Aptos:size=14",
										"JetBrainsMono Nerd Font:size=16" };


static char c000000[]			= "#000000";

static char normfgcolor[]		= "#bbbbbb";
static char normbgcolor[]		= "#222222";
static char normbordercolor[]	= "#444444";
static char normfloatcolor[]	= "#444444";

static char selfgcolor[]		= "#eeeeee";
static char selbgcolor[]		= "#005577";
static char selbordercolor[]	= "#61afef";
static char selfloatcolor[]		= "#c678dd";

static char ltsymbolfgcolor[]	= "#ffdd00";
static char ltsymbolbgcolor[]	= "#222222";

static char stbuttonfgcolor[]	= "#61afef";
static char stbuttonbgcolor[]	= "#222222";

static char *colors[][4]	= {
	/*			fg	bg	border	*/
	[SchemeNorm] 		= { normfgcolor,		normbgcolor,		normbordercolor,	normfloatcolor	},
	[SchemeSel]			= { selfgcolor,			selbgcolor,			selbordercolor,		selfloatcolor },
	[SchemeLtSymbol]	= { ltsymbolfgcolor,	ltsymbolbgcolor,	c000000,			c000000	},
	[SchemeStButton]	= { stbuttonfgcolor,	stbuttonbgcolor,	c000000,			c000000	},
};

static const unsigned int baralpha 		= 0xd0;
static const unsigned int borderalpha 	= OPAQUE;

static const unsigned int alphas[][4]	= {
	/*	fg	bg	border*/
	[SchemeNorm] 		= { OPAQUE, baralpha, borderalpha, borderalpha },
	[SchemeSel]			= { OPAQUE, baralpha, borderalpha, borderalpha },
	[SchemeLtSymbol]	= { OPAQUE, baralpha, borderalpha, borderalpha },
	[SchemeStButton]	= { OPAQUE, baralpha, borderalpha, borderalpha },
};


static const char *const autostart[] = {
	"/usr/libexec/polkit-gnome-authentication-agent-1", NULL,
	"dunst", NULL,
	"nitrogen", "--restore", NULL,
	"picom", "-b", NULL,
	"slstatus", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] 		= { "·", "·", "·", "·", "·", "·" , "·", "·"	};
static const char *alttags[] 	= { "", "", "", "", "", "", "", "" };

static const unsigned int ulinepad		= 5;
static const unsigned int ulinestroke	= 2;
static const unsigned int ulinevoffset	= 0;
static const int ulineall 				= 0;

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	{ .class = "Lxappearance", .isfloating = 1, .floatpos = "50% 50% -1w -1h" },
	{ .class = "Alacritty", .tags = 1 << 0, .noswallow = 1 },
	{ .class = "Firefox", .tags = 1 << 1 },
	{ .class = "Thunar", .tags = 1 << 2 },
	{ .class = "spterm", .scratchkey = 't', .isfloating = 1, .floatpos = "50% 50% 85% 85%" },
	{ .title = "Event Tester", .noswallow = 1, .isfloating = 1 },
};

/* layout(s) */
static const float mfact			= 0.50; 
static const int nmaster			= 1;
static const int resizehints 		= 0;
static const int lockfullscreen 	= 1;
static int floatposgrid_x			= 5;
static int floatposgrid_y			= 5;
static const int swallowfloating	= 0;
static const char *toggle_float_pos	= "50% 50% 80% 80%";

#define FORCE_VSPLIT 1
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol	arrange function */
	{ "[]=",	tile },	
	{ "><>",	NULL },	
	{ "[M]",	monocle },
	{ "[@]",	spiral },
	{ "[\\]",	dwindle },
	{ "H[]",	deck },
	{ "TTT",	bstack },
	{ "===",	bstackhoriz },
	{ "HHH",	grid },
	{ "###",	nrowgrid },
	{ "---",	horizgrid },
	{ ":::",	gaplessgrid },
	{ "|M|",	centeredmaster },
	{ ">M>",	centeredfloatingmaster },
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,						KEY,	view,			{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,			KEY,	toggleview,		{.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,				KEY,	tag,			{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask,	KEY,	toggletag,		{.ui = 1 << TAG} },

/* commands */
static const char *termcmd[]		= { "alacritty", NULL };
static const char *roficmd[]		= { "rofi", "-show", "drun", NULL };
static const char *webcmd[]			= { "firefox", NULL };
static const char *fmcmd[]			= { "thunar", NULL };
static const char *colpickcmd[]		= { "colorpicker", NULL };

static const char *blupcmd[]		= { "brightness", "--inc", NULL };
static const char *bldowncmd[]		= { "brightness", "--decc", NULL };

static const char *volmutecmd[]		= { "volume", "--toggle", NULL };
static const char *micmutecmd[]		= { "volume", "--toggle-mic", NULL };
static const char *voldowncmd[]		= { "volume", "--dec", NULL };
static const char *volupcmd[]		= { "volume", "--inc", NULL };

static const char *sptermcmd[] 		= { "t", "alacritty", "--class", "spterm,spterm", NULL }; 

static const Key keys[] = {
	/* modifier				key							function			argument */
	{ MODKEY,				XK_Return, 					spawn,				{.v = termcmd } },
	{ MODKEY,				XK_space,					spawn,				{.v = roficmd } },
	{ MODKEY,				XK_w,						spawn,				{.v = webcmd } },
	{ MODKEY,				XK_e,						spawn,				{.v = fmcmd } },
	{ MODKEY|ShiftMask,		XK_p,						spawn,				{.v = colpickcmd } },
	{ 0,					XF86XK_MonBrightnessUp,		spawn,				{.v = blupcmd } },
	{ 0,					XF86XK_MonBrightnessDown,	spawn,				{.v = bldowncmd } },
	{ 0, 					XF86XK_AudioMute, 			spawn, 				{.v = volmutecmd } },
	{ 0, 					XF86XK_AudioMicMute, 		spawn, 				{.v = micmutecmd } },
	{ 0, 					XF86XK_AudioLowerVolume, 	spawn, 				{.v = voldowncmd } },
	{ 0, 					XF86XK_AudioRaiseVolume, 	spawn,				{.v = volupcmd } },
	{ MODKEY,				XK_grave,					togglescratch,		{.v = sptermcmd } },
	{ MODKEY,				XK_j,						focusstack,			{.i = +1 } },
	{ MODKEY,				XK_k,						focusstack,			{.i = -1 } },
	{ MODKEY,				XK_i,						incnmaster,			{.i = +1 } },
	{ MODKEY,				XK_d,						incnmaster,			{.i = -1 } },
	{ MODKEY,				XK_h,						setmfact,			{.f = -0.05} },
	{ MODKEY,				XK_l,						setmfact,			{.f = +0.05} },
	{ MODKEY|ShiftMask,		XK_h,						setcfact,			{.f = +0.25} },
	{ MODKEY|ShiftMask,		XK_l,						setcfact,			{.f = -0.25} },
	{ MODKEY|ShiftMask,		XK_o,						setcfact,			{.f =	0.00} },
	{ MODKEY,				XK_equal,					incrgaps,			{.i = +1 } },
	{ MODKEY,				XK_minus,					incrgaps,			{.i = -1 } },
	{ MODKEY|ShiftMask,		XK_g,						togglegaps,			{0} },
	{ MODKEY|ControlMask,	XK_g,						defaultgaps,		{0} },
	{ MODKEY,				XK_q,						killclient,			{0} },
	{ MODKEY|ShiftMask,		XK_q,						quit,				{0} },
	{ MODKEY|ShiftMask,		XK_r,						quit,				{1} }, 
	{ MODKEY,				XK_t,						setlayout,			{.v = &layouts[0]} },
	{ MODKEY,				XK_f,						setlayout,			{.v = &layouts[1]} },
	{ MODKEY,				XK_m,						setlayout,			{.v = &layouts[2]} },
	{ MODKEY,				XK_b,						setlayout,			{.v = &layouts[6]} },
	{ MODKEY,				XK_g,						setlayout,			{.v = &layouts[11]} },
	{ MODKEY,				XK_comma,					cyclelayout,    	{.i = -1 } },
	{ MODKEY,           	XK_period, 					cyclelayout,    	{.i = +1 } },
	{ MODKEY|ShiftMask,		XK_space,					togglefloating,		{0} },
	{ MODKEY|ShiftMask,		XK_b,						togglebar,			{0} },
	{ MODKEY|ShiftMask,		XK_f,						togglefullscr,		{0} },
	{ MODKEY,				XK_F5,						xrdb,				{0} },
	{ MODKEY,				XK_0,						view,				{.ui = ~0 } },
	{ MODKEY|ShiftMask,		XK_0,						tag,				{.ui = ~0 } },
	{ MODKEY,				XK_Right,					viewnext,			{0} },
	{ MODKEY,				XK_Left,					viewprev,			{0} },
	{ MODKEY|ShiftMask,		XK_Right,					tagtonext,			{0} },
	{ MODKEY|ShiftMask,		XK_Left,					tagtoprev,			{0} },
	TAGKEYS(				XK_1,											0)
	TAGKEYS(				XK_2,											1)
	TAGKEYS(				XK_3,											2)
	TAGKEYS(				XK_4,											3)
	TAGKEYS(				XK_5,											4)
	TAGKEYS(				XK_6,											5)
	TAGKEYS(				XK_7,											6)
	TAGKEYS(				XK_8,											7)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click			event mask	button		function		argument */
	{ ClkButton,		0,			Button1,	spawn,			{.v = roficmd } },
	{ ClkLtSymbol,		0,			Button1,	setlayout,		{0} },
	{ ClkLtSymbol,		0,			Button3,	setlayout,		{.v = &layouts[2]} },
	{ ClkWinTitle,		0,			Button2,	zoom,			{0} },
	{ ClkStatusText,	0,			Button2,	spawn,			{.v = termcmd } },
	{ ClkClientWin,		MODKEY,		Button1,	movemouse,		{0} },
	{ ClkClientWin,		MODKEY,		Button2,	togglefloating, {0} },
	{ ClkClientWin,		MODKEY,		Button3,	resizemouse,	{0} },
	{ ClkTagBar,		0,			Button1,	view,			{0} },
	{ ClkTagBar,		0,			Button3,	toggleview,		{0} },
	{ ClkTagBar,		MODKEY,		Button1,	tag,			{0} },
	{ ClkTagBar,		MODKEY,		Button3,	toggletag,		{0} },
};

