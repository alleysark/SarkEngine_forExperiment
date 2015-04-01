#ifndef __INPUT_H__
#define __INPUT_H__

#include "core.h"
#include <Windows.h>
#include <hash_map>
#include <list>
#include <functional>

namespace sark {

	// input static class management input system as general
	// *note: when you are trying to debugging this class, 
	//			some problems can be occurred about window message skipping
	class Input {
	public:

		// --------------------------------------------
		class Keyboard {
		public:
			typedef uint8 KeyCode;

			// virtual keys of windows(it can be assigned by other system codes)
			static const KeyCode CODE_NONE = 0;

			static const KeyCode CODE_BACK = VK_BACK;
			static const KeyCode CODE_TAB = VK_TAB;
			static const KeyCode CODE_SPACE = VK_SPACE;
			static const KeyCode CODE_ENTER = VK_RETURN;

			static const KeyCode CODE_ESC = VK_ESCAPE;
			static const KeyCode CODE_SHIFT = VK_SHIFT;
			static const KeyCode CODE_CTRL = VK_CONTROL;
			static const KeyCode CODE_ALT = VK_MENU;
			static const KeyCode CODE_INSERT = VK_INSERT;
			static const KeyCode CODE_DELETE = VK_DELETE;

			static const KeyCode CODE_NUMLOCK = VK_NUMLOCK;
			static const KeyCode CODE_CAPSLOCK = VK_CAPITAL;
			static const KeyCode CODE_SCROLL = VK_SCROLL;
			static const KeyCode CODE_HANENG = VK_HANGUL;
			static const KeyCode CODE_HANJA = VK_HANJA;

			static const KeyCode CODE_PAGEUP = VK_PRIOR;
			static const KeyCode CODE_PAGEDOWN = VK_NEXT;
			static const KeyCode CODE_END = VK_END;
			static const KeyCode CODE_HOME = VK_HOME;

			static const KeyCode CODE_UP = VK_UP;
			static const KeyCode CODE_DOWN = VK_DOWN;
			static const KeyCode CODE_LEFT = VK_LEFT;
			static const KeyCode CODE_RIGHT = VK_RIGHT;

			static const KeyCode CODE_F1 = VK_F1;
			static const KeyCode CODE_F2 = VK_F2;
			static const KeyCode CODE_F3 = VK_F3;
			static const KeyCode CODE_F4 = VK_F4;
			static const KeyCode CODE_F5 = VK_F5;
			static const KeyCode CODE_F6 = VK_F6;
			static const KeyCode CODE_F7 = VK_F7;
			static const KeyCode CODE_F8 = VK_F8;
			static const KeyCode CODE_F9 = VK_F9;
			static const KeyCode CODE_F10 = VK_F10;
			static const KeyCode CODE_F11 = VK_F11;
			static const KeyCode CODE_F12 = VK_F12;

			static const KeyCode CODE_0 = '0'; //0x30
			static const KeyCode CODE_1 = '1'; //0x31
			static const KeyCode CODE_2 = '2'; //0x32
			static const KeyCode CODE_3 = '3'; //0x33
			static const KeyCode CODE_4 = '4'; //0x34
			static const KeyCode CODE_5 = '5'; //0x35
			static const KeyCode CODE_6 = '6'; //0x36
			static const KeyCode CODE_7 = '7'; //0x37
			static const KeyCode CODE_8 = '8'; //0x38
			static const KeyCode CODE_9 = '9'; //0x39

			static const KeyCode CODE_A = 'A'; //0x41
			static const KeyCode CODE_B = 'B'; //0x42
			static const KeyCode CODE_C = 'C'; //0x43
			static const KeyCode CODE_D = 'D'; //0x44
			static const KeyCode CODE_E = 'E'; //0x45
			static const KeyCode CODE_F = 'F'; //0x46
			static const KeyCode CODE_G = 'G'; //0x47
			static const KeyCode CODE_H = 'H'; //0x48
			static const KeyCode CODE_I = 'I'; //0x49
			static const KeyCode CODE_J = 'J'; //0x4A
			static const KeyCode CODE_K = 'K'; //0x4B
			static const KeyCode CODE_L = 'L'; //0x4C
			static const KeyCode CODE_M = 'M'; //0x4D
			static const KeyCode CODE_N = 'N'; //0x4E
			static const KeyCode CODE_O = 'O'; //0x4F
			static const KeyCode CODE_P = 'P'; //0x50
			static const KeyCode CODE_Q = 'Q'; //0x51
			static const KeyCode CODE_R = 'R'; //0x52
			static const KeyCode CODE_S = 'S'; //0x53
			static const KeyCode CODE_T = 'T'; //0x54
			static const KeyCode CODE_U = 'U'; //0x55
			static const KeyCode CODE_V = 'V'; //0x56
			static const KeyCode CODE_W = 'W'; //0x57
			static const KeyCode CODE_X = 'X'; //0x58
			static const KeyCode CODE_Y = 'Y'; //0x59
			static const KeyCode CODE_Z = 'Z'; //0x5A

			static const KeyCode CODE_NUMPAD0 = VK_NUMPAD0;
			static const KeyCode CODE_NUMPAD1 = VK_NUMPAD1;
			static const KeyCode CODE_NUMPAD2 = VK_NUMPAD2;
			static const KeyCode CODE_NUMPAD3 = VK_NUMPAD3;
			static const KeyCode CODE_NUMPAD4 = VK_NUMPAD4;
			static const KeyCode CODE_NUMPAD5 = VK_NUMPAD5;
			static const KeyCode CODE_NUMPAD6 = VK_NUMPAD6;
			static const KeyCode CODE_NUMPAD7 = VK_NUMPAD7;
			static const KeyCode CODE_NUMPAD8 = VK_NUMPAD8;
			static const KeyCode CODE_NUMPAD9 = VK_NUMPAD9;

			static const KeyCode CODE_MULTIPLY = VK_MULTIPLY;
			static const KeyCode CODE_ADD = VK_ADD;
			static const KeyCode CODE_SUBTRACT = VK_SUBTRACT;
			static const KeyCode CODE_DIVIDE = VK_DIVIDE;
			static const KeyCode CODE_DECIMAL = VK_DECIMAL;

			typedef uint32 KeyComb;
			typedef std::function<void(void)> KeyCombHandler;
			typedef std::hash_map<KeyComb, KeyCombHandler> KeyCombHandlerContainer;

		private:
			static const uinteger MAX_CODE_COUNT = 4;
			uinteger mCodeCount;
			KeyCode mCodes[MAX_CODE_COUNT];
			KeyCombHandlerContainer mCombHandlers;
		public:
			Keyboard();

			// register key combination handler.
			// it overrides the handler for same code combination.
			void RegisterKeyCombHandler(KeyCombHandler keyCombHandler,
				KeyCode code1, KeyCode code2 = CODE_NONE, KeyCode code3 = CODE_NONE, KeyCode code4 = CODE_NONE);

			void Down(KeyCode code);
			void Up(KeyCode code);

		private:
			KeyComb BuildKeyComb(KeyCode codes[MAX_CODE_COUNT]);
		};


		// ------------	mouse properties	------------------
		class Mouse {
		public:
			enum MouseState {
				STATE_NONE = 0,

				STATE_MOVEMENT = 1 << 0,
				STATE_LBUTTON = 1 << 1,
				STATE_MBUTTON = 1 << 2,
				STATE_RBUTTON = 1 << 3,

				STATE_LDRAGGING = 1 << 4,
				STATE_MDRAGGING = 1 << 5,
				STATE_RDRAGGING = 1 << 6,

				STATE_WHEEL = 1 << 7,
			};

			enum MouseEvent {
				EVENT_MOVE = 0,
				
				// drag: button down and move
				EVENT_LBUTTON_DRAG,
				EVENT_MBUTTON_DRAG,
				EVENT_RBUTTON_DRAG,

				// button down
				EVENT_LBUTTON_DOWN,
				EVENT_MBUTTON_DOWN,
				EVENT_RBUTTON_DOWN,

				// button up
				EVENT_LBUTTON_UP,
				EVENT_MBUTTON_UP,
				EVENT_RBUTTON_UP,

				// click: button down and up
				EVENT_LBUTTON_CLICK,
				EVENT_MBUTTON_CLICK,
				EVENT_RBUTTON_CLICK,

				EVENT_WHEEL,

				// only for counting
				EVENT_COUNT
			};

			typedef std::function<void(const Position2&, real)> EventHandler;

		private:
			uint32 mActiveStates;
			EventHandler mEventHandlers[EVENT_COUNT];

		public:
			Mouse();

			// register mouse event handler.
			// it overrides the handler for the same event
			void RegisterMouseHandler(MouseEvent eventCode, EventHandler handler);

			void MovementUpdate(uint32 x, uint32 y);
			void LButtonUpdate(uint32 x, uint32 y);
			void MButtonUpdate(uint32 x, uint32 y);
			void RButtonUpdate(uint32 x, uint32 y);
			void WheelUpdate(uint32 x, uint32 y, real delta);

			bool IsLButtonDown() const;
			bool IsMButtonDown() const;
			bool IsRButtonDown() const;

			bool IsLDragging() const;
			bool IsMDragging() const;
			bool IsRDragging() const;

		private:
			inline void StateOn(const MouseState& state) {
				mActiveStates |= state;
			}
			inline void StateOff(const MouseState& state) {
				mActiveStates ^= state;
			}

			inline void TriggerEvent(MouseEvent evt, const uint32& x, const uint32& y, const real& extra) {
				if (mEventHandlers[evt])
					mEventHandlers[evt](Position2(x, y), extra);
			}
		};

	private:
		// it can't be instantiated
		Input() {}
		Input(const Input& input) {}

	public:
		static Keyboard keyboard;
		static Mouse mouse;

	};

}
#endif