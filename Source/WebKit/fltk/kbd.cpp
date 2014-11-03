/*
WebkitFLTK
Copyright (C) 2014 Lauri Kasanen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "config.h"

#include "kbd.h"
#include <WindowsKeyboardCodes.h>

#include <FL/Enumerations.H>
#include <X11/keysym.h>
#include <X11/Xlib.h>

String keyidfor(const int in) {

	switch(in) {
		case XK_Tab:
		case XK_ISO_Left_Tab:
			return "U+0009";
		case XK_Delete:
			return "U+007F";
		case XK_BackSpace:
			return "U+0008";
		case XK_Return:
			return "Enter"; // They actually string-match to this...
	}

	return XKeysymToString(in);
}

unsigned winkeyfor(const int in) {

	if (in >= XK_a && in <= XK_z)
		return VK_A + (in - XK_a);
	if (in >= XK_A && in <= XK_Z)
		return VK_A + (in - XK_A);

	switch (in) {
		case XK_KP_0:
			return VK_NUMPAD0;// (60) Numeric keypad 0 key
		case XK_KP_1:
			return VK_NUMPAD1;// (61) Numeric keypad 1 key
		case XK_KP_2:
			return	VK_NUMPAD2; // (62) Numeric keypad 2 key
		case XK_KP_3:
			return VK_NUMPAD3; // (63) Numeric keypad 3 key
		case XK_KP_4:
			return VK_NUMPAD4; // (64) Numeric keypad 4 key
		case XK_KP_5:
			return VK_NUMPAD5; //(65) Numeric keypad 5 key
		case XK_KP_6:
			return VK_NUMPAD6; // (66) Numeric keypad 6 key
		case XK_KP_7:
			return VK_NUMPAD7; // (67) Numeric keypad 7 key
		case XK_KP_8:
			return VK_NUMPAD8; // (68) Numeric keypad 8 key
		case XK_KP_9:
			return VK_NUMPAD9; // (69) Numeric keypad 9 key
		case XK_KP_Multiply:
			return VK_MULTIPLY; // (6A) Multiply key
		case XK_KP_Add:
			return VK_ADD; // (6B) Add key
		case XK_KP_Subtract:
			return VK_SUBTRACT; // (6D) Subtract key
		case XK_KP_Decimal:
			return VK_DECIMAL; // (6E) Decimal key
		case XK_KP_Divide:
			return VK_DIVIDE; // (6F) Divide key

		case XK_KP_Page_Up:
			return VK_PRIOR; // (21) PAGE UP key
		case XK_KP_Page_Down:
			return VK_NEXT; // (22) PAGE DOWN key
		case XK_KP_End:
			return VK_END; // (23) END key
		case XK_KP_Home:
			return VK_HOME; // (24) HOME key
		case XK_KP_Left:
			return VK_LEFT; // (25) LEFT ARROW key
		case XK_KP_Up:
			return VK_UP; // (26) UP ARROW key
		case XK_KP_Right:
			return VK_RIGHT; // (27) RIGHT ARROW key
		case XK_KP_Down:
			return VK_DOWN; // (28) DOWN ARROW key

		case XK_BackSpace:
			return VK_BACK; // (08) BACKSPACE key
		case XK_ISO_Left_Tab:
		case XK_Tab:
			return VK_TAB; // (09) TAB key
		case XK_Clear:
			return VK_CLEAR; // (0C) CLEAR key
		case XK_ISO_Enter:
		case XK_KP_Enter:
		case XK_Return:
			return VK_RETURN; //(0D) Return key

			// VK_SHIFT (10) SHIFT key
			// VK_CONTROL (11) CTRL key

		case XK_Menu:
			return VK_APPS;  // (5D) Applications key (Natural keyboard)

			// VK_MENU (12) ALT key

		case XK_Pause:
			return VK_PAUSE; // (13) PAUSE key
		case XK_Caps_Lock:
			return VK_CAPITAL; // (14) CAPS LOCK key
		case XK_Kana_Lock:
		case XK_Kana_Shift:
			return VK_KANA; // (15) Input Method Editor (IME) Kana mode
		case XK_Hangul:
			return VK_HANGUL; // VK_HANGUL (15) IME Hangul mode
			// VK_JUNJA (17) IME Junja mode
			// VK_FINAL (18) IME final mode
		case XK_Hangul_Hanja:
			return VK_HANJA; // (19) IME Hanja mode
		case XK_Kanji:
			return VK_KANJI; // (19) IME Kanji mode
		case XK_Escape:
			return VK_ESCAPE; // (1B) ESC key
			// VK_CONVERT (1C) IME convert
			// VK_NONCONVERT (1D) IME nonconvert
			// VK_ACCEPT (1E) IME accept
			// VK_MODECHANGE (1F) IME mode change request
		case XK_space:
			return VK_SPACE; // (20) SPACEBAR
		case XK_Page_Up:
			return VK_PRIOR; // (21) PAGE UP key
		case XK_Page_Down:
			return VK_NEXT; // (22) PAGE DOWN key
		case XK_End:
			return VK_END; // (23) END key
		case XK_Home:
			return VK_HOME; // (24) HOME key
		case XK_Left:
			return VK_LEFT; // (25) LEFT ARROW key
		case XK_Up:
			return VK_UP; // (26) UP ARROW key
		case XK_Right:
			return VK_RIGHT; // (27) RIGHT ARROW key
		case XK_Down:
			return VK_DOWN; // (28) DOWN ARROW key
		case XK_Select:
			return VK_SELECT; // (29) SELECT key
		case XK_Print:
			return VK_SNAPSHOT; // (2C) PRINT SCREEN key
		case XK_Execute:
			return VK_EXECUTE;// (2B) EXECUTE key
		case XK_Insert:
		case XK_KP_Insert:
			return VK_INSERT; // (2D) INS key
		case XK_Delete:
		case XK_KP_Delete:
			return VK_DELETE; // (2E) DEL key
		case XK_Help:
			return VK_HELP; // (2F) HELP key
		case XK_0:
		case XK_parenright:
			return VK_0;	//	(30) 0) key
		case XK_1:
		case XK_exclam:
			return VK_1; //  (31) 1 ! key
		case XK_2:
		case XK_at:
			return VK_2; //  (32) 2 & key
		case XK_3:
		case XK_numbersign:
			return VK_3; //case '3': case '#';
		case XK_4:
		case XK_dollar: //  (34) 4 key '$';
			return VK_4;
		case XK_5:
		case XK_percent:
			return VK_5; //  (35) 5 key  '%'
		case XK_6:
		case XK_asciicircum:
			return VK_6; //  (36) 6 key  '^'
		case XK_7:
		case XK_ampersand:
			return VK_7; //  (37) 7 key  case '&'
		case XK_8:
		case XK_asterisk:
			return VK_8; //  (38) 8 key  '*'
		case XK_9:
		case XK_parenleft:
			return VK_9; //  (39) 9 key '('
		case XK_Meta_L:
			return VK_LWIN; // (5B) Left Windows key (Microsoft Natural keyboard)
		case XK_Meta_R:
			return VK_RWIN; // (5C) Right Windows key (Natural keyboard)
			// VK_SLEEP (5F) Computer Sleep key
			// VK_SEPARATOR (6C) Separator key
			// VK_SUBTRACT (6D) Subtract key
			// VK_DECIMAL (6E) Decimal key
			// VK_DIVIDE (6F) Divide key
			// handled by key code above

		case XK_Num_Lock:
			return VK_NUMLOCK; // (90) NUM LOCK key

		case XK_Scroll_Lock:
			return VK_SCROLL; // (91) SCROLL LOCK key

		case XK_Shift_L:
			return VK_LSHIFT; // (A0) Left SHIFT key
		case XK_Shift_R:
			return VK_RSHIFT; // (A1) Right SHIFT key
		case XK_Control_L:
			return VK_LCONTROL; // (A2) Left CONTROL key
		case XK_Control_R:
			return VK_RCONTROL; // (A3) Right CONTROL key
		case XK_Alt_L:
			return VK_LMENU; // (A4) Left MENU key
		case XK_Alt_R:
			return VK_RMENU; // (A5) Right MENU key

			// VK_BROWSER_BACK (A6) Windows 2000/XP: Browser Back key
			// VK_BROWSER_FORWARD (A7) Windows 2000/XP: Browser Forward key
			// VK_BROWSER_REFRESH (A8) Windows 2000/XP: Browser Refresh key
			// VK_BROWSER_STOP (A9) Windows 2000/XP: Browser Stop key
			// VK_BROWSER_SEARCH (AA) Windows 2000/XP: Browser Search key
			// VK_BROWSER_FAVORITES (AB) Windows 2000/XP: Browser Favorites key
			// VK_BROWSER_HOME (AC) Windows 2000/XP: Browser Start and Home key
			// VK_VOLUME_MUTE (AD) Windows 2000/XP: Volume Mute key
			// VK_VOLUME_DOWN (AE) Windows 2000/XP: Volume Down key
			// VK_VOLUME_UP (AF) Windows 2000/XP: Volume Up key
			// VK_MEDIA_NEXT_TRACK (B0) Windows 2000/XP: Next Track key
			// VK_MEDIA_PREV_TRACK (B1) Windows 2000/XP: Previous Track key
			// VK_MEDIA_STOP (B2) Windows 2000/XP: Stop Media key
			// VK_MEDIA_PLAY_PAUSE (B3) Windows 2000/XP: Play/Pause Media key
			// VK_LAUNCH_MAIL (B4) Windows 2000/XP: Start Mail key
			// VK_LAUNCH_MEDIA_SELECT (B5) Windows 2000/XP: Select Media key
			// VK_LAUNCH_APP1 (B6) Windows 2000/XP: Start Application 1 key
			// VK_LAUNCH_APP2 (B7) Windows 2000/XP: Start Application 2 key

			// VK_OEM_1 (BA) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ';:' key
		case XK_semicolon:
		case XK_colon:
			return VK_OEM_1; //case ';': case ':': return 0xBA;
			// VK_OEM_PLUS (BB) Windows 2000/XP: For any country/region, the '+' key
		case XK_plus:
		case XK_equal:
			return VK_OEM_PLUS; //case '=': case '+': return 0xBB;
			// VK_OEM_COMMA (BC) Windows 2000/XP: For any country/region, the ',' key
		case XK_comma:
		case XK_less:
			return VK_OEM_COMMA; //case ',': case '<': return 0xBC;
			// VK_OEM_MINUS (BD) Windows 2000/XP: For any country/region, the '-' key
		case XK_minus:
		case XK_underscore:
			return VK_OEM_MINUS; //case '-': case '_': return 0xBD;
			// VK_OEM_PERIOD (BE) Windows 2000/XP: For any country/region, the '.' key
		case XK_period:
		case XK_greater:
			return VK_OEM_PERIOD; //case '.': case '>': return 0xBE;
			// VK_OEM_2 (BF) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '/?' key
		case XK_slash:
		case XK_question:
			return VK_OEM_2; //case '/': case '?': return 0xBF;
			// VK_OEM_3 (C0) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '`~' key
		case XK_asciitilde:
		case XK_quoteleft:
			return VK_OEM_3; //case '`': case '~': return 0xC0;
			// VK_OEM_4 (DB) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '[{' key
		case XK_bracketleft:
		case XK_braceleft:
			return VK_OEM_4; //case '[': case '{': return 0xDB;
			// VK_OEM_5 (DC) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '\|' key
		case XK_backslash:
		case XK_bar:
			return VK_OEM_5; //case '\\': case '|': return 0xDC;
			// VK_OEM_6 (DD) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ']}' key
		case XK_bracketright:
		case XK_braceright:
			return VK_OEM_6; // case ']': case '}': return 0xDD;
			// VK_OEM_7 (DE) Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
		case XK_quoteright:
		case XK_quotedbl:
			return VK_OEM_7; // case '\'': case '"': return 0xDE;
			// VK_OEM_8 (DF) Used for miscellaneous characters; it can vary by keyboard.
			// VK_OEM_102 (E2) Windows 2000/XP: Either the angle bracket key or the backslash key on the RT 102-key keyboard
			// VK_PROCESSKEY (E5) Windows 95/98/Me, Windows NT 4.0, Windows 2000/XP: IME PROCESS key
			// VK_PACKET (E7) Windows 2000/XP: Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT,SendInput, WM_KEYDOWN, and WM_KEYUP
			// VK_ATTN (F6) Attn key
			// VK_CRSEL (F7) CrSel key
			// VK_EXSEL (F8) ExSel key
			// VK_EREOF (F9) Erase EOF key
			// VK_PLAY (FA) Play key
			// VK_ZOOM (FB) Zoom key
			// VK_NONAME (FC) Reserved for future use
			// VK_PA1 (FD) PA1 key
			// VK_OEM_CLEAR (FE) Clear key
		case XK_F1:
		case XK_F2:
		case XK_F3:
		case XK_F4:
		case XK_F5:
		case XK_F6:
		case XK_F7:
		case XK_F8:
		case XK_F9:
		case XK_F10:
		case XK_F11:
		case XK_F12:
		case XK_F13:
		case XK_F14:
		case XK_F15:
		case XK_F16:
		case XK_F17:
		case XK_F18:
		case XK_F19:
		case XK_F20:
		case XK_F21:
		case XK_F22:
		case XK_F23:
		case XK_F24:
			return VK_F1 + (in - XK_F1);
		case XK_VoidSymbol:
			return VK_PROCESSKEY;
	}

	return 0;
}
