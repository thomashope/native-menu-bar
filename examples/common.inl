#include "../native_menu_bar.h"

#define UNUSED(x) (void)(x)

/* Menu item IDs */
nmb_Handle g_hFileNew = 0;
nmb_Handle g_hFileOpen = 0;
nmb_Handle g_hFileSave = 0;
nmb_Handle g_hFileEnabler = 0;
nmb_Handle g_hFileEnablee = 0;
nmb_Handle g_hEditCopy = 0;
nmb_Handle g_hEditPaste = 0;
nmb_Handle g_hHelpAbout = 0;

void createMenuBar(void* nativeWindowHandle)
{
    nmb_Handle hMenuBar = nmb_setup(nativeWindowHandle);
    
    nmb_Handle hFileMenu = nmb_appendMenu(hMenuBar, "File");
    nmb_Handle hEditMenu = nmb_appendMenu(hMenuBar, "Edit");
    nmb_Handle hHelpMenu = nmb_appendMenu(NULL, "Help öäĂĹŸ");

    /* File menu */
    g_hFileNew = nmb_appendMenuItem(hFileMenu, "New");
    g_hFileOpen = nmb_appendMenuItem(hFileMenu, "Open...");
    g_hFileSave = nmb_appendMenuItem(hFileMenu, "Save");
    nmb_appendMenuItem(hFileMenu, "Check Me");
    nmb_appendSeparator(hFileMenu);
    g_hFileEnabler = nmb_appendMenuItem(hFileMenu, "Enabler");
    g_hFileEnablee = nmb_appendMenuItem(hFileMenu, "Enablee");

    /* Edit menu */
    g_hEditCopy = nmb_appendMenuItem(hEditMenu, "Copy");
    g_hEditPaste = nmb_appendMenuItem(hEditMenu, "Paste");

    nmb_Handle submenu1 = nmb_insertMenu(hFileMenu, 1, "Inserted Submenu");
    nmb_appendMenuItem(submenu1, "Item 1");
    nmb_appendMenuItem(submenu1, "Item 2");
    nmb_appendMenuItem(submenu1, "Item 3");
    nmb_insertMenuItem(submenu1, 1, "Inserted Menu");
    nmb_insertSeparator(hFileMenu, 2);
    nmb_insertSeparator(hFileMenu, 1);

    nmb_Handle submenu2 = nmb_appendMenu(hEditMenu, "Submenu");
    nmb_appendMenuItem(submenu2, "Submenu Item 1");

    /* Help menu */
    g_hHelpAbout = nmb_appendMenuItem(hHelpMenu, "About...");
}

void handleEvents()
{
    nmb_Event e;
    while (nmb_pollEvent(&e))
    {
        if (e.sender == g_hFileNew)
        {
            printf("New file selected\n");
        }
        else if (e.sender == g_hFileOpen)
        {
            printf("Open file selected\n");
        }
        else if (e.sender == g_hFileSave)
        {
            printf("Save file selected\n");
        }
        else if (e.sender == g_hFileEnabler)
        {
            nmb_setMenuItemEnabled(g_hFileEnablee, !nmb_isMenuItemEnabled(g_hFileEnablee));
        }
        else if (e.sender == g_hEditCopy)
        {
            printf("Copy selected\n");
        }
        else if (e.sender == g_hEditPaste)
        {
            printf("Paste selected\n");
        }
        else if (e.sender == g_hHelpAbout)
        {
            printf("SDL2 Window Demo\nVersion 1.0\n");
        }
        else
        {
            nmb_setMenuItemChecked(e.sender, !nmb_isMenuItemChecked(e.sender));
        }
    }
}
