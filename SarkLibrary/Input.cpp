#include "Input.h"
#include <algorithm>

namespace sarklib{

	Input::Keyboard::Keyboard(){}

	bool Input::Keyboard::RegisterKeyCombHandler(KeyCombHandler keyCombHandler,
		KeyCode code1, KeyCode code2, KeyCode code3, KeyCode code4){
		KeyCodeContainer tmpCodeList;
		tmpCodeList.push_back(code1);
		tmpCodeList.push_back(code2);
		tmpCodeList.push_back(code3);
		tmpCodeList.push_back(code4);
		KeyComb comb = BuildKeyComb(tmpCodeList);
		
		KeyCombHandlerContainer::iterator finder = mCombHandlers.find(comb);
		if (finder != mCombHandlers.end())
			return false;
		mCombHandlers[comb] = keyCombHandler;
		return true;
	}

	Input::Keyboard::KeyComb Input::Keyboard::BuildKeyComb(KeyCodeContainer& codes){
		codes.sort();
		KeyComb combCode = 0;
		integer keycodeBit = 8 * sizeof(KeyCode);
		for (KeyCodeContainer::iterator itr = codes.begin(); itr != codes.end(); itr++){
			combCode = combCode << keycodeBit;
			combCode |= (*itr);
		}
		return combCode;
	}

	void Input::Keyboard::Down(KeyCode code){
		// push input code into the mCodes only if code is not existed in mCodes
		KeyCodeContainer::iterator itr = std::find(mCodes.begin(), mCodes.end(), code);
		if (itr == mCodes.end())
			mCodes.push_back(code);

		// if the conbination of mCodes is registered in mCombHandlers, then invoke handler
		KeyComb comb = BuildKeyComb(mCodes);
		KeyCombHandlerContainer::iterator finder = mCombHandlers.find(comb);
		if (finder != mCombHandlers.end())
			finder->second();
	}
	void Input::Keyboard::Up(KeyCode code){
		// pull input code from mCodes
		KeyCodeContainer::iterator itr = std::find(mCodes.begin(), mCodes.end(), code);
		if (itr != mCodes.end())
			mCodes.erase(itr);
	}


	Input::Keyboard Input::keyboard;
	Input::Mouse Input::mouse;
}