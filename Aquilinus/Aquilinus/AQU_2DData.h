/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio 
Copyright © 2014 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown and Neil Schneider

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/
#ifndef AQU_2DDATA
#define AQU_2DDATA

#pragma region fields
/**
* Simple color enumeration for the simple Aquilinus color scheme.
***/
enum AquilinusColor
{
	Color0,
	Color1,
	Color2,
	Color3,
	Color4
};

/**
* Color scheme structure for 5 colors + creator info.
***/
struct ColorScheme
{
	UINT uColor[5];
	WCHAR szName[32];
	WCHAR szLover[32];
	WCHAR szLink[128];
};

/**
* Basic PNG image data as stored in the IHDR chunk.
***/
struct ImageData
{
	DWORD dwWidth;
	DWORD dwHeight;
	BYTE chBitDepth;
	BYTE chColorType;
	BYTE chCompressionMethod;
	BYTE chFilterMethod;
	BYTE chInterlaceMethod;
};
#pragma endregion

#pragma region colors

#define AQUILINUS_NUMBER_OF_COLOR_SCHEMES 37

/**
* The constant color schemes to be set in the ColorSchemes enumeration.
***/
ColorScheme g_colorSchemes[] =
{
	/*http://www.colourlovers.com/palette/1681868/Agassian_War_Dogs
	Name="Agassian War Dogs" by rockstatic >
	<SolidColorSwatch Color="#B2783F" />
	<SolidColorSwatch Color="#90877C" />
	<SolidColorSwatch Color="#AFAFAF" />
	<SolidColorSwatch Color="#EAEAEA" />
	<SolidColorSwatch Color="#575757" />*/
	{ 0xB2783F,
	0x90877C,
	0xAFAFAF,
	0xEAEAEA,
	0x575757,
	L"Agassian War Dogs",
	L"rockstatic",
	L"http://www.colourlovers.com/palette/1681868/Agassian_War_Dogs"
	},

	/*http://www.colourlovers.com/palette/1415764/Ancient_Armoury
	// Ancient Armoury by rafferty
	<SolidColorBrush x:Key="Brush1" Color="0xA1A76B" />
	<SolidColorBrush x:Key="Brush2" Color="0x74745A" />
	<SolidColorBrush x:Key="Brush3" Color="0x2B2A25" />
	<SolidColorBrush x:Key="Brush4" Color="0xFFD1BA" />
	<SolidColorBrush x:Key="Brush5" Color="0xBB8D6B" />*/
	{ 0xA1A76B,
	0x74745A,
	0x2B2A25,
	0xFFD1BA,
	0xBB8D6B,
	L"Ancient Armoury",
	L"rafferty",
	L"http://www.colourlovers.com/palette/1415764/Ancient_Armoury"
	},

	/*http://www.colourlovers.com/palette/2549121/Ancient_History
	// Ancient History by Duchess
	<SolidColorBrush x:Key="Brush1" Color="0x2C0000" />
	<SolidColorBrush x:Key="Brush2" Color="0x711A2D" />
	<SolidColorBrush x:Key="Brush3" Color="0xCD766D" />
	<SolidColorBrush x:Key="Brush4" Color="0xF9B98D" />
	<SolidColorBrush x:Key="Brush5" Color="0xF39562" />*/
	{ 0x2C0000,
	0x711A2D,
	0xCD766D,
	0xF9B98D,
	0xF39562,
	L"Ancient History",
	L"Duchess",
	L"http://www.colourlovers.com/palette/2549121/Ancient_History"
	},

	/*http://www.colourlovers.com/palette/2441571/ancient_maps
	// Ancient Maps by redcurrant
	<SolidColorBrush x:Key="Brush1" Color="0xE6E0D3" />
	<SolidColorBrush x:Key="Brush2" Color="0x9AABA1" />
	<SolidColorBrush x:Key="Brush3" Color="0x6E737A" />
	<SolidColorBrush x:Key="Brush4" Color="0xA19191" />
	<SolidColorBrush x:Key="Brush5" Color="0x967575" />*/
	{ 0xE6E0D3,
	0x9AABA1,
	0x6E737A,
	0xA19191,
	0x967575,
	L"Ancient Maps",
	L"redcurrant",
	L"http://www.colourlovers.com/palette/2441571/ancient_maps"
	},

	/*http://www.colourlovers.com/palette/3363853/%E2%80%A2Ancient_Scent%E2%80%A2
	// *Ancient Scent* by Duklida
	<SolidColorBrush x:Key="Brush1" Color="0xA6A0A5" />
	<SolidColorBrush x:Key="Brush2" Color="0xF4F4E9" />
	<SolidColorBrush x:Key="Brush3" Color="0xFBB694" />
	<SolidColorBrush x:Key="Brush4" Color="0xBA0281" />
	<SolidColorBrush x:Key="Brush5" Color="0x662D54" />*/
	{ 0xA6A0A5,
	0xF4F4E9,
	0xFBB694,
	0xBA0281,
	0x662D54,
	L"*Ancient Scent*",
	L"Duklida",
	L"http://www.colourlovers.com/palette/3363853/%E2%80%A2Ancient_Scent%E2%80%A2"
	},

	/*http://www.colourlovers.com/palette/3305300/%E2%80%A2Ancient_Souls%E2%80%A2
	// *Ancient Souls* by Duklida
	<SolidColorBrush x:Key="Brush1" Color="0x4A4059" />
	<SolidColorBrush x:Key="Brush2" Color="0xA18093" />
	<SolidColorBrush x:Key="Brush3" Color="0xEC979A" />
	<SolidColorBrush x:Key="Brush4" Color="0xFDCAC9" />
	<SolidColorBrush x:Key="Brush5" Color="0xE8E8DE" />*/
	{ 0x4A4059,
	0xA18093,
	0xEC979A,
	0xFDCAC9,
	0xE8E8DE,
	L"*Ancient Souls*",
	L"Duklida",
	L"http://www.colourlovers.com/palette/3305300/%E2%80%A2Ancient_Souls%E2%80%A2"
	},

	/*http://www.colourlovers.com/palette/218745/Antica_Pompeii
	// Antica Pompeii By vermilion
	// Sandyby Chorellik : ECD393
	// Spring Rainfall by Oread : 9EC5AA
	// mediterraneo by vermilion : 17989C
	// la Befana by vermilion : 2E1802
	// penelope by Oona : C01818*/
	{ 0xECD393,
	0x9EC5AA,
	0x17989C,
	0x2E1802,
	0xC01818,
	L"Antica Pompeii",
	L"vermilion",
	L"http://www.colourlovers.com/palette/218745/Antica_Pompeii"
	},

	/*http://www.colourlovers.com/palette/3204414/%E2%80%A2Antique_Prophecy%E2%80%A2
	// Antique Prophecy by Duklida
	<SolidColorBrush x:Key="Brush1" Color="0xD8E1DE" />
	<SolidColorBrush x:Key="Brush2" Color="0xA7CCB0" />
	<SolidColorBrush x:Key="Brush3" Color="0x7E9C92" />
	<SolidColorBrush x:Key="Brush4" Color="0x2B2E45" />
	<SolidColorBrush x:Key="Brush5" Color="0xE6CF00" />*/
	{ 0xD8E1DE,
	0xA7CCB0,
	0x7E9C92,
	0x2B2E45,
	0xE6CF00,
	L"*Antique Prophecy*",
	L"Duklida",
	L"http://www.colourlovers.com/palette/3204414/%E2%80%A2Antique_Prophecy%E2%80%A2"
	},

	/*http://www.colourlovers.com/palette/2890270/Aquileia_mosaics
	// Aquileia mosaics By LenAine
	<SolidColorBrush x:Key="Brush1" Color="0x363444" />
	<SolidColorBrush x:Key="Brush2" Color="0x9AAA8D" />
	<SolidColorBrush x:Key="Brush3" Color="0xF0E5D3" />
	<SolidColorBrush x:Key="Brush4" Color="0xD6B48B" />
	<SolidColorBrush x:Key="Brush5" Color="0x935246" />*/
	{ 0x363444,
	0x9AAA8D,
	0xF0E5D3,
	0xD6B48B,
	0x935246,
	L"Aquileia Mosaics",
	L"LenAine",
	L"http://www.colourlovers.com/palette/2890270/Aquileia_mosaics"
	},

	/*http://www.colourlovers.com/palette/3459585/Aquilinus
	// Aquilinus by EisernSchild
	<SolidColorBrush x:Key="Brush1" Color="0x981A00" />
	<SolidColorBrush x:Key="Brush2" Color="0xCCCC99" />
	<SolidColorBrush x:Key="Brush3" Color="0xC28416" />
	<SolidColorBrush x:Key="Brush4" Color="0x3D3737" />
	<SolidColorBrush x:Key="Brush5" Color="0x7F060B" />*/
	{ 0x981A00,
	0xCCCC99,
	0xC28416,
	0x3D3737,
	0x7F060B,
	L"Aquilinus",
	L"EisernSchild",
	L"http://www.colourlovers.com/palette/3459585/Aquilinus"
	},

	/*http://www.colourlovers.com/palette/179975/autumn_in_Rome
	Name="autumn in Rome" by webfruits>
	<SolidColorSwatch Color="#522E16" />
	<SolidColorSwatch Color="#AD4D2B" />
	<SolidColorSwatch Color="#D2A23E" />
	<SolidColorSwatch Color="#E7D3B3" />
	<SolidColorSwatch Color="#C4AF83" />*/
	{ 0x522E16,
	0xAD4D2B,
	0xD2A23E,
	0xE7D3B3,
	0xC4AF83,
	L"Autumn in Rome",
	L"webfruits",
	L"http://www.colourlovers.com/palette/179975/autumn_in_Rome"
	},

	/*http://www.colourlovers.com/palette/3240116/%E2%80%A2Autumn_Rainbow%E2%80%A2
	// Autumn Rainbow by Duklida
	<SolidColorBrush x:Key="Brush1" Color="0xF2F2E3" />
	<SolidColorBrush x:Key="Brush2" Color="0xFDBD69" />
	<SolidColorBrush x:Key="Brush3" Color="0xFA6957" />
	<SolidColorBrush x:Key="Brush4" Color="0x60A589" />
	<SolidColorBrush x:Key="Brush5" Color="0x554141" />*/
	{ 0xF2F2E3,
	0xFDBD69,
	0xFA6957,
	0x60A589,
	0x554141,
	L"Autumn Rainbow",
	L"Duklida",
	L"http://www.colourlovers.com/palette/3240116/%E2%80%A2Autumn_Rainbow%E2%80%A2"
	},

	/*http://www.colourlovers.com/palette/68185/burning_empire
	Name="burning empire" by deliquescence>
	<SolidColorSwatch Color="0xFFEE94" />
	<SolidColorSwatch Color="0xE87909" />
	<SolidColorSwatch Color="0x803839" />
	<SolidColorSwatch Color="0x4D1A29" />
	<SolidColorSwatch Color="0x593D3D" />*/
	{ 0xFFEE94,
	0xE87909,
	0x803839,
	0x4D1A29,
	0x593D3D,
	L"Burning Empire",
	L"deliquescence",
	L"http://www.colourlovers.com/palette/68185/burning_empire"
	},

	/*http://www.colourlovers.com/palette/1683630/Caesar
	// Caesar by chay33
	<SolidColorBrush x:Key="Brush1" Color="0xC4B299" />
	<SolidColorBrush x:Key="Brush2" Color="0x739B9E" />
	<SolidColorBrush x:Key="Brush3" Color="0x705800" />
	<SolidColorBrush x:Key="Brush4" Color="0x4A0808" />
	<SolidColorBrush x:Key="Brush5" Color="0x2B1900" />
	*/
	{ 0xC4B299,
	0x739B9E,
	0x705800,
	0x4A0808,
	0x2B1900,
	L"Caesar",
	L"chay33",
	L"http://www.colourlovers.com/palette/1683630/Caesar"
	},

	/*http://www.colourlovers.com/palette/2643876/Caesars_Toga
	// Caesar's Toga by Lucie Lafrance
	<SolidColorBrush x:Key="Brush1" Color="0x47225F" />
	<SolidColorBrush x:Key="Brush2" Color="0xA01528" />
	<SolidColorBrush x:Key="Brush3" Color="0xF6F3F4" />
	<SolidColorBrush x:Key="Brush4" Color="0xF5DD19" />
	<SolidColorBrush x:Key="Brush5" Color="0x0C0C0C" />*/
	{ 0x47225F,
	0xA01528,
	0xF6F3F4,
	0xF5DD19,
	0x0C0C0C,
	L"Caesar's Toga",
	L"Lafrance",
	L"http://www.colourlovers.com/palette/2643876/Caesars_Toga"
	},

	/*http://www.colourlovers.com/palette/1412275/caesar_and_cleopatra
	// Caesar and Cleopatra by earlgrey
	<SolidColorBrush x:Key="Brush1" Color="0xD7BDA1" />
	<SolidColorBrush x:Key="Brush2" Color="0xD49D94" />
	<SolidColorBrush x:Key="Brush3" Color="0x9C848C" />
	<SolidColorBrush x:Key="Brush4" Color="0x070605" />
	<SolidColorBrush x:Key="Brush5" Color="0x251A1B" />*/
	{ 0xD7BDA1,
	0xD49D94,
	0x9C848C,
	0x070605,
	0x251A1B,
	L"Caesar and Cleopatra",
	L"earlgrey",
	L"http://www.colourlovers.com/palette/1412275/caesar_and_cleopatra"
	},

	/*http://www.colourlovers.com/palette/652634/Colosseum
	// Colosseum by Tuna
	<SolidColorBrush x:Key="Brush1" Color="0xF7E4CD" />
	<SolidColorBrush x:Key="Brush2" Color="0xD9CAB2" />
	<SolidColorBrush x:Key="Brush3" Color="0xAD9A8E" />
	<SolidColorBrush x:Key="Brush4" Color="0x967878" />
	<SolidColorBrush x:Key="Brush5" Color="0xFA0505" />*/
	{ 0xF7E4CD,
	0xD9CAB2,
	0xAD9A8E,
	0x967878,
	0xFA0505,
	L"Colosseum",
	L"Tuna",
	L"http://www.colourlovers.com/palette/652634/Colosseum"
	},

	/*http://www.colourlovers.com/palette/1438291/ColoursOfTheLand_RC
	// Colours of the Land RC by rafferty
	<SolidColorBrush x:Key="Brush1" Color="0x747184" />
	<SolidColorBrush x:Key="Brush2" Color="0xBF8F41" />
	<SolidColorBrush x:Key="Brush3" Color="0xC95A14" />
	<SolidColorBrush x:Key="Brush4" Color="0x800000" />
	<SolidColorBrush x:Key="Brush5" Color="0x2B0000" />*/
	{ 0x747184,
	0xBF8F41,
	0xC95A14,
	0x800000,
	0x2B0000,
	L"Colours of the Land",
	L"rafferty",
	L"http://www.colourlovers.com/palette/1438291/ColoursOfTheLand_RC"
	},

	/*http://www.colourlovers.com/palette/3440867/Final_Say
	// Final Say By Duchess
	<SolidColorBrush x:Key="Brush1" Color="0x2D0017" />
	<SolidColorBrush x:Key="Brush2" Color="0x54104C" />
	<SolidColorBrush x:Key="Brush3" Color="0x8E5BB3" />
	<SolidColorBrush x:Key="Brush4" Color="0x92A2E0" />
	<SolidColorBrush x:Key="Brush5" Color="0xB8BFFF" />*/
	{ 0x2D0017,
	0x54104C,
	0x8E5BB3,
	0x92A2E0,
	0xB8BFFF,
	L"Final Say",
	L"Duchess",
	L"http://www.colourlovers.com/palette/3440867/Final_Say"
	},

	/*http://www.colourlovers.com/palette/3353986/graceful_rome
	Name="graceful rome" by coloraura">
	<SolidColorSwatch Color="#CCDAFF" />
	<SolidColorSwatch Color="#FFDDCC" />
	<SolidColorSwatch Color="#E0AB2D" />
	<SolidColorSwatch Color="#212020" />
	<SolidColorSwatch Color="#880047" />*/
	{ 0xCCDAFF,
	0xFFDDCC,
	0xE0AB2D,
	0x212020,
	0x880047,
	L"Graceful Rome",
	L"coloraura",
	L"http://www.colourlovers.com/palette/3353986/graceful_rome"
	},

	/*http://www.colourlovers.com/palette/788789/Last_Day_of_Pompeii
	// Last Day of Pompeii By WNDINTER
	// Magma by WNDINTER : F04202
	// Roman Empire by WNDINTER : E3A405
	// Fawn doll by angela.schippers : E6E0AA
	// August 23, 79 AD by WNDINTER : 916A3D
	// Vesuvius by WNDINTER : 261C0F*/
	{ 0xF04202,
	0xE3A405,
	0xE6E0AA,
	0x916A3D,
	0x261C0F,
	L"Last Day of Pompeii",
	L"WNDINTER",
	L"http://www.colourlovers.com/palette/788789/Last_Day_of_Pompeii"
	},

	/*http://www.colourlovers.com/palette/857991/Nero_Claudius_Caesar
	// Nero Claudius Caesar by Colorguru
	<SolidColorBrush x:Key="Brush1" Color="0xE88C61" />
	<SolidColorBrush x:Key="Brush2" Color="0xC44B4B" />
	<SolidColorBrush x:Key="Brush3" Color="0x852C40" />
	<SolidColorBrush x:Key="Brush4" Color="0x401129" />
	<SolidColorBrush x:Key="Brush5" Color="0x1F0716" />*/
	{ 0xE88C61,
	0xC44B4B,
	0x852C40,
	0x401129,
	0x1F0716,
	L"Nero Claudius Caesar",
	L"Colorguru",
	L"http://www.colourlovers.com/palette/857991/Nero_Claudius_Caesar"
	},

	/*http://www.colourlovers.com/palette/134940/Pompeii
	// Pompeii By skylaced
	// Rediscover by skylaced : 8FBABF
	// Horizon by petalthrow : 6090A8
	// Electrik Escape by shore : 607890
	// nook by lunalein : 484860
	// tabasco by lunalein : D84830o*/
	{ 0x8FBABF,
	0x6090A8,
	0x607890,
	0x484860,
	0xD84830,
	L"Pompeii",
	L"skylaced",
	L"http://www.colourlovers.com/palette/134940/Pompeii"
	},

	/*http://www.colourlovers.com/palette/2382224/Roman_Blue
	// Roman Blue By QueenElisheba
	<SolidColorBrush x:Key="Brush1" Color="0x291D0D" />
	<SolidColorBrush x:Key="Brush2" Color="0x656752" />
	<SolidColorBrush x:Key="Brush3" Color="0xD0C9AF" />
	<SolidColorBrush x:Key="Brush4" Color="0x68AA8F" />
	<SolidColorBrush x:Key="Brush5" Color="0x7B9A9D" />*/
	{ 0x291D0D,
	0x656752,
	0xD0C9AF,
	0x68AA8F,
	0x7B9A9D,
	L"Roman Blue",
	L"QueenElisheba",
	L"http://www.colourlovers.com/palette/2382224/Roman_Blue"
	},

	/*http://www.colourlovers.com/palette/941694/Roman_Dusk
	// Roman Dusk By Phantom Ink
	<SolidColorBrush x:Key="Brush1" Color="0xA8A089" />
	<SolidColorBrush x:Key="Brush2" Color="0x694F38" />
	<SolidColorBrush x:Key="Brush3" Color="0x74818A" />
	<SolidColorBrush x:Key="Brush4" Color="0xDDC088" />
	<SolidColorBrush x:Key="Brush5" Color="0x563320" />*/
	{ 0xA8A089,
	0x694F38,
	0x74818A,
	0xDDC088,
	0x563320,
	L"Roman Dusk",
	L"Phantom Ink",
	L"http://www.colourlovers.com/palette/941694/Roman_Dusk"
	},

	/*http://www.colourlovers.com/palette/478790/Roman_Empire
	// Roman Empire By Cucutor
	<SolidColorBrush x:Key="Brush1" Color="0x1E132B" />
	<SolidColorBrush x:Key="Brush2" Color="0x45162E" />
	<SolidColorBrush x:Key="Brush3" Color="0x967747" />
	<SolidColorBrush x:Key="Brush4" Color="0xCCA262" />
	<SolidColorBrush x:Key="Brush5" Color="0xF2C074" />*/
	{ 0x1E132B,
	0x45162E,
	0x967747,
	0xCCA262,
	0xF2C074,
	L"Roman Empire",
	L"Cucutor",
	L"http://www.colourlovers.com/palette/478790/Roman_Empire"
	},

	/*http://www.colourlovers.com/palette/3004953/Roman_fresco
	// Roman fresco By haluk
	// Railyard by leadwhite : E8CEAD
	// Feel the Sand by Any Palacios : E1C184
	// Sandy by erinblue_eyes : 8F916C
	// seeing my thoughts by onlyalice : AC650D
	// 821906 by Mykee : 821906*/
	{ 0xE8CEAD,
	0xE1C184,
	0x8F916C,
	0xAC650D,
	0x821906,
	L"Roman Fresco",
	L"haluk",
	L"http://www.colourlovers.com/palette/3004953/Roman_fresco"
	},

	/*http://www.colourlovers.com/palette/1303328/Roman_Garden
	// Roman Garden By gwenjcl
	// Geranium by gwenjcl : A02134
	// silk blush by shatterheart : D2767B
	// Long Winter's Tanby oldja : F9F0C9
	// oh no i burnt it! by phill : B1AC86
	// Circeo by gwenjcl : 99886C*/
	{ 0xA02134,
	0xD2767B,
	0xF9F0C9,
	0xB1AC86,
	0x99886C,
	L"Roman Garden",
	L"gwenjcl",
	L"http://www.colourlovers.com/palette/1303328/Roman_Garden"
	},

	/*http://www.colourlovers.com/palette/1798761/Romanum
	// Romanum By kcnlsn
	// hello by kcnlsn : C5945A
	// Big big bone by Sylke : FFEDB9
	// Mask by NicolesLovesong : 3C3808
	// Slim Limb by DesignCoyote : 7D4A2D
	// dusty rose thakoon by samanthahahn : BEA58F*/
	{ 0xC5945A,
	0xFFEDB9,
	0x3C3808,
	0x7D4A2D,
	0xBEA58F,
	L"Romanum",
	L"kcnlsn",
	L"http://www.colourlovers.com/palette/1798761/Romanum"
	},

	/*http://www.colourlovers.com/palette/3198402/%E2%80%A2Rome_Courtyard%E2%80%A2
	Name="•Rome Courtyard•" by Duklida
	<SolidColorSwatch Color="0xFBE9D3" />
	<SolidColorSwatch Color="0xCAC1A0" />
	<SolidColorSwatch Color="0xA8A244" />
	<SolidColorSwatch Color="0xC15D43" />
	<SolidColorSwatch Color="0x372525" />*/
	{ 0xFBE9D3,
	0xCAC1A0,
	0xA8A244,
	0xC15D43,
	0x372525,
	L"*Rome Courtyard*",
	L"Duklida",
	L"http://www.colourlovers.com/palette/3198402/%E2%80%A2Rome_Courtyard%E2%80%A2"
	},

	/*http://www.colourlovers.com/palette/1502224/Rome_in_Sepia
	Name="Rome in Sepia" by lenazap
	<SolidColorSwatch Color="#E4D4BB" />
	<SolidColorSwatch Color="#C7B199" />
	<SolidColorSwatch Color="#B39D88" />
	<SolidColorSwatch Color="#7D6F66" />
	<SolidColorSwatch Color="#3E3634" />*/
	{ 0xE4D4BB,
	0xC7B199,
	0xB39D88,
	0x7D6F66,
	0x3E3634,
	L"Rome in Sepia",
	L"lenazap",
	L"http://www.colourlovers.com/palette/1502224/Rome_in_Sepia"
	},

	/*http://www.colourlovers.com/palette/683789/Romulus
	Name="Romulus" by mary_cherry_17 >
	<SolidColorSwatch Color="#CCCCBE" />
	<SolidColorSwatch Color="#83898C" />
	<SolidColorSwatch Color="#485C67" />
	<SolidColorSwatch Color="#8B0400" />
	<SolidColorSwatch Color="#670101" />*/
	{ 0xCCCCBE,
	0x83898C,
	0x485C67,
	0x8B0400,
	0x670101,
	L"Romulus",
	L"mary_cherry_17",
	L"http://www.colourlovers.com/palette/683789/Romulus"
	},

	/*http://www.colourlovers.com/palette/2277174/tiberius
	Name="tiberius" by iambubby>
	<SolidColorSwatch Color="#CDD6B4" />
	<SolidColorSwatch Color="#899683" />
	<SolidColorSwatch Color="#686B60" />
	<SolidColorSwatch Color="#33524B" />
	<SolidColorSwatch Color="#043536" />*/
	{ 0xCDD6B4,
	0x899683,
	0x686B60,
	0x33524B,
	0x043536,
	L"Tiberius",
	L"iambubby",
	L"http://www.colourlovers.com/palette/2277174/tiberius"
	},

	/*http://www.colourlovers.com/palette/2411327/Trajans_Market
	Name="Trajan's Market" by bigrocket >
	<SolidColorSwatch Color="#D2D9E9" />
	<SolidColorSwatch Color="#282825" />
	<SolidColorSwatch Color="#98764A" />
	<SolidColorSwatch Color="#734B3D" />
	<SolidColorSwatch Color="#96816C" />*/
	{ 0xD2D9E9,
	0x282825,
	0x98764A,
	0x734B3D,
	0x96816C,
	L"Trajan's Market",
	L"bigrocket",
	L"http://www.colourlovers.com/palette/2411327/Trajans_Market"
	},

	/*http://www.colourlovers.com/palette/1974985/Vacanze_romane
	// Vacanze romane by plch
	// Girlie Purple by pinkgirlie : 5C164C
	// sensualby ellymarinova : B1060E
	// hollywood princessby shocktheworld : FF001B
	// Sprintingby stepoutofthecar : D3D674
	// Moonby Nice Autumn : F4F2D5*/
	{ 0x5C164C,
	0xB1060E,
	0xFF001B,
	0xD3D674,
	0xF4F2D5,
	L"Vacanze Romane",
	L"plch",
	L"http://www.colourlovers.com/palette/1974985/Vacanze_romane"
	},

	/*http://www.colourlovers.com/palette/2305105/Vesuves
	// Vesuves By deobogdo
	// vesuves orange by deobogdo : D85B36
	// Thinking Out Loud by DreamsComeTrue : 803723
	// spring blue by waniey Rz : A4BABE
	// vizantijska noc by deobogdo : 2C4547
	// extreme teal by HartnSoul : 101C1B*/
	{ 0xD85B36,
	0x803723,
	0xA4BABE,
	0x2C4547,
	0x101C1B,
	L"Vesuves",
	L"deobogdo",
	L"http://www.colourlovers.com/palette/2305105/Vesuves"
	},

	/*http://www.colourlovers.com/palette/248355/Via_Rome
	Name="Via Rome" by _183 >
	<SolidColorSwatch Color="#A2ECCC" />
	<SolidColorSwatch Color="#CCCC99" />
	<SolidColorSwatch Color="#FFFFCC" />
	<SolidColorSwatch Color="#E69966" />
	<SolidColorSwatch Color="#CC3300" />*/
	{ 0xA2ECCC,
	0xCCCC99,
	0xFFFFCC,
	0xE69966,
	0xCC3300,
	L"Via Rome",
	L"_183",
	L"http://www.colourlovers.com/palette/248355/Via_Rome"
	},
};

#pragma endregion

#endif
