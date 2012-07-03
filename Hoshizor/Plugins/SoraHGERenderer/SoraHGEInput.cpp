#include "SoraHGEInput.h"
#include "SoraCore.h"

namespace sora {

	SoraHGEInput::SoraHGEInput() {
		hge = hgeCreate(HGE_VERSION);
		if(!hge)
			throw SORA_EXCEPTION(SystemException, "Error creating hge");
	}

	SoraHGEInput::~SoraHGEInput() {
		hge->Release();
	}

	void SoraHGEInput::getMousePos(float* x, float* y) {
		hge->Input_GetMousePos(x, y);
	}

	void SoraHGEInput::setMousePos(float x, float y) {
		hge->Input_SetMousePos(x, y);
	}

	float SoraHGEInput::getMousePosX() {
		float x, y;
		hge->Input_GetMousePos(&x, &y);
		return x;
	}

	float SoraHGEInput::getMousePosY() {
		float x, y;
		hge->Input_GetMousePos(&x, &y);
		return y;
	}

	int SoraHGEInput::getMouseWheel() {
		return hge->Input_GetMouseWheel();
	}

	bool SoraHGEInput::isMouseOver() {
		return hge->Input_IsMouseOver();
	}

	bool SoraHGEInput::keyDown(int32 key) {
		return hge->Input_GetKeyState(key);
	}

	bool SoraHGEInput::keyUp(int32 key) {
		return !hge->Input_GetKeyState(key);
	}

	int32 SoraHGEInput::getKeyState(int32 key) {
		return hge->Input_GetKeyState(key);
	}

	char* SoraHGEInput::getKeyName(int32 key) {
		return hge->Input_GetKeyName(key);
	}

	bool SoraHGEInput::getKeyEvent(SoraKeyEvent& ev) {
		hgeInputEvent oev;
		bool result = hge->Input_GetEvent(&oev);
		ev.chr = oev.chr;
		ev.flags = oev.flags;
		ev.key = oev.key;
		ev.type = oev.type;
		ev.wheel = oev.wheel;
		ev.x = oev.x;
		ev.y = oev.y;
		return result;
	}

	bool SoraHGEInput::joyKeyPressed(int key) {
		return hge->Input_GetDIKey(key, SORA_JOYSTICK_PRESSED);
	}

	bool SoraHGEInput::joyKeyDown(int key) {
		return hge->Input_GetDIKey(key, SORA_JOYSTICK_DOWN);
	}

	bool SoraHGEInput::joyKeyUp(int key) {
		return hge->Input_GetDIKey(key, SORA_JOYSTICK_UP);
	}

	bool SoraHGEInput::joyKeyState(int key, unsigned char state) {
		return hge->Input_GetDIKey(key, state);
	}

	bool SoraHGEInput::setJoyKey(int key) {
		return hge->Input_SetDIKey(key);
	}

	bool SoraHGEInput::hasJoy() {
		return hge->Input_HaveJoy();
	}

#ifdef SORA_AUTOMATIC_REGISTER
	SORA_STATIC_RUN_CODE(SoraCore::Instance()->registerInput(new SoraHGEInput));
#endif

} // namespace sora
