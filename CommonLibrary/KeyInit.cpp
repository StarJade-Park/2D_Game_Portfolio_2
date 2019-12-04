#include <..\psklib\GameKeyInput.h>

void InitAllKeys( )
{
	GameKeyInput::CreateKey(L"Up", VK_UP);
	GameKeyInput::CreateKey(L"Down", VK_DOWN);
	GameKeyInput::CreateKey(L"Left", VK_LEFT);
	GameKeyInput::CreateKey(L"Right", VK_RIGHT);

	GameKeyInput::CreateKey(L"ChangeToPrev", 'A');
	GameKeyInput::CreateKey(L"ChangeToNext", 'S');

	GameKeyInput::CreateKey(L"Jump", 'Z');
	GameKeyInput::CreateKey(L"Shoot", 'X');
	GameKeyInput::CreateKey(L"Attack", 'C');
	GameKeyInput::CreateKey(L"Bomb", 'V');

	GameKeyInput::CreateKey(L"Space", VK_SPACE);

	GameKeyInput::CreateKey(L"CamZoomIn", 'O');
	GameKeyInput::CreateKey(L"CamZoomOut", 'P');

	//GameKeyInput::CreateKey(L"mRotX+", 'Q');
	//GameKeyInput::CreateKey(L"mRotX-", 'W');
	//GameKeyInput::CreateKey(L"mRotY+", 'E');
	//GameKeyInput::CreateKey(L"mRotY-", 'R');
	//GameKeyInput::CreateKey(L"mRotZ+", 'T');
	//GameKeyInput::CreateKey(L"mRotZ-", 'Y');

	GameKeyInput::CreateKey(L"MLB", VK_LBUTTON);
	GameKeyInput::CreateKey(L"MRB", VK_RBUTTON);

	GameKeyInput::CreateKey(L"Enter", VK_RETURN); // enter key
	GameKeyInput::CreateKey(L"Reset", VK_BACK);
	GameKeyInput::CreateKey(L"JustDoIt", VK_TAB); // debug용 scene 전환
	GameKeyInput::CreateKey(L"Debug", VK_F8); // debug용 랜더링 키
}
