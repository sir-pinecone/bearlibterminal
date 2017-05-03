/*
 * Main.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: Cfyz
 */

#include "Common.hpp"
#include <vector>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#include <sys/param.h>
#endif

struct TestEntry
{
	const char* name;
	void (*func)();
};

TERMINAL_TAKE_CARE_OF_WINMAIN

int main()
{
#if defined(__APPLE__)
	char path[MAXPATHLEN] = {0};
	uint32_t bufsize = MAXPATHLEN;
	_NSGetExecutablePath(path, &bufsize);
	for (int i = bufsize-1; i >= 0; i--)
	{
		if (path[i] == '/')
		{
				path[i] = '\0';
				break;
		}
	}
	chdir(path);
#endif

	terminal_open();

	auto reset = []()
	{
		terminal_set(
			"window: size=80x25, cellsize=auto, title='Omni: menu';"
			"font: default;"
			"input: filter={keyboard}"
		);
		terminal_color("white");
	};

	std::vector<TestEntry> entries =
	{
		{"Basic output", TestBasicOutput},
		{"Default font", TestDefaultFont},
		{"Tilesets", TestTilesets},
		{"Sprites", TestSprites},
		{"Manual cellsize", TestManualCellsize},
		{"Auto-generated tileset", TestAutoGenerated},
		{"Multiple fonts", TestMultipleFonts},
		{"Text alignment", TestTextAlignment},
		{"Formatted log", TestFormattedLog},
		{"Layers", TestLayers},
		{"Extended 1: basics", TestExtendedBasics},
		//{"Extended 2: inter-layer animation", TestExtendedInterlayer},
		{"Extended 2: smooth scroll", TestExtendedSmoothScroll},
		{"Dynamic sprites", TestDynamicSprites},
		{"Speed", TestSpeed},
		{"Input 1: keyboard", TestKeyboard},
		{"Input 2: mouse", TestMouse},
		{"Input 3: text input", TestTextInput},
		{"Input 4: filtering", TestInputFiltering},
		{"Window resizing", TestWindowResize},
		{"Examining cell contents", TestPick}
	};

	reset();

	while (true)
	{
		terminal_clear();
		for (size_t i=0; i<entries.size(); i++)
		{
			char shortcut = i < 9? '1'+i: 'a'+(i-9);
			terminal_printf(2, 1+i, "[color=orange]%c.[/color] %s%s", shortcut, entries[i].func? "": "[color=gray]", entries[i].name);
		}
		terminal_printf(2, 23, "[color=orange]ESC.[/color] Exit");
		terminal_printf_ext(77, 22, 0, 0, TK_ALIGN_RIGHT, "library version %s", terminal_get("version"));
		terminal_printf_ext(77, 23, 0, 0, TK_ALIGN_RIGHT, "http://wyrd.name/en:bearlibterminal");
		terminal_refresh();

		int key = terminal_read();

		if (key == TK_ESCAPE || key == TK_CLOSE)
		{
			break;
		}
		else if ((key >= TK_1 && key <= TK_9) || (key >= TK_A && key <= TK_Z))
		{
			int index = key >= TK_1? (key-TK_1): 9+(key-TK_A);
			if (index >= 0 && index < entries.size() && entries[index].func)
			{
				entries[index].func();
				reset();
			}
		}
	}
	terminal_close();
	return 0;
}