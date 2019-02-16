#include "PCH.h"

void CSceneGame::InitInstance()
{
	ASE_THIS(CField)->InitInstance();
}

void CSceneGame::Update(double tickCount)
{
	//	1.	Object Update
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		//std::cout << "VK_UP" << std::endl;
		ASE_THIS(CField)->AddY(-1);
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		//std::cout << "VK_DOWN" << std::endl;
		ASE_THIS(CField)->AddY(1);
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		//std::cout << "VK_LEFT" << std::endl;
		ASE_THIS(CField)->AddX(-1);
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		//std::cout << "VK_RIGHT" << std::endl;
		ASE_THIS(CField)->AddX(1);

	if (ASE_THIS(CField)->GetPlayerX() < 0)
		ASE_THIS(CField)->SetPlayerX(0);
	if (ASE_THIS(CField)->GetPlayerX() >= CField::FIELD_CELL_SIZE)
		ASE_THIS(CField)->SetPlayerX(CField::FIELD_CELL_SIZE - 1);

	if (ASE_THIS(CField)->GetPlayerY() < 0)
		ASE_THIS(CField)->SetPlayerY(0);
	if (ASE_THIS(CField)->GetPlayerY() >= CField::FIELD_CELL_SIZE)
		ASE_THIS(CField)->SetPlayerY(CField::FIELD_CELL_SIZE - 1);
}

void CSceneGame::Render()
{
	//	2.	Object Render
	system("cls");
	ASE_THIS(CField)->Render();
}
