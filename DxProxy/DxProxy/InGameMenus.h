#pragma once

#define COLOR_WHITE    D3DCOLOR_ARGB(255, 255, 255, 255)
#define COLOR_RED      D3DCOLOR_ARGB(255, 255, 0, 0)
#define COLOR_GREEN    D3DCOLOR_ARGB(255, 0, 255, 0)
#define COLOR_BLUE     D3DCOLOR_ARGB(255, 0, 0, 255)
#define COLOR_LIGHTRED D3DCOLOR_ARGB(255, 255, 128, 128)

#define COLOR_MENU_TEXT     COLOR_WHITE
#define COLOR_TEXT_SHADOW   D3DCOLOR_ARGB(255, 64, 64, 64)
#define COLOR_MENU_BORDER   D3DCOLOR_ARGB(255, 255, 128, 128)
#define COLOR_MENU_DISABLED D3DCOLOR_ARGB(255, 255, 64, 64)
#define COLOR_MENU_ENABLED  D3DCOLOR_ARGB(255, 64, 255, 64)
#define COLOR_QUICK_SETTING D3DCOLOR_ARGB(255, 128, 196, 128)
#define COLOR_INFO_POPUP    D3DCOLOR_ARGB(255, 128, 255, 128)
#define COLOR_HASH_LINE     D3DCOLOR_ARGB(255,255,255,0)

#define MENU_ITEM_SEPARATION  40
#define MENU_SELECT_ANIMATION_SPEED 0.8f
#define MENU_SCROLL_BOTTOM 12


class MenuBuilder
{
public:
	MenuBuilder(D3DProxyDevice *device);
	
	void DrawItem(const char *text, D3DCOLOR color=COLOR_MENU_TEXT);
	void DrawItem(std::string text, D3DCOLOR color=COLOR_MENU_TEXT);
	void AddItem(std::string text, std::function<void()> onHover);
	void AddItem(std::string text, D3DCOLOR color, std::function<void()> onHover);
	void AddButton(std::string text, D3DCOLOR color, std::function<void()> onPick);
	void AddButton(std::string text, std::function<void()> onPick);
	void AddNavigation(std::string text, std::function<void()> menuHandler);
	void AddGameKeypress(std::string text, byte *binding);
	void AddKeybind(std::string text, InputBindingRef *binding);
	void AddAdjustment(const char *formatString, float *value, float defaultValue, float rate, std::function<void()> onChange=[](){});
	
	void ResetDrawPosition();
	int GetDrawPositionTop();
	int GetEntryCount();

private:
	D3DProxyDevice *device;
	int menuConstructionCurrentEntry;
	RECT drawPosition;
};

class MenuState
{
public:
	MenuState(D3DProxyDevice *device);
	void Reset();
	
	/// Which menu item is currently selected
	int selectedIndex;
	
	/// Index of the top-most visible item, reflecting how far the menu is scrolled
	int scrollOffset;
	
	/// Offset between the menu selection, and where the border is drawn.
	float animationOffset;
	
	/// Main menu border top height.
	float borderTopHeight;
	
	std::function<void()> handleCurrentMenu;
	
private:
	D3DProxyDevice *device;
};
