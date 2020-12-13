#include "afxwin.h"
#include "resource.h"
#include <string.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

//==============================================================================
//Global 

vector<vector<int>> pole(24,vector<int>(30));
vector<vector<int>> AI_pole(24,vector<int>(30));
vector<vector<int>> open(24,vector<int>(30));
int bomb = 0;
int m = 9;
int n = 9;
int max_bomb = 10;
int endgame = 0;
bool first_step = 1;
bool dblclk = 0;
string c;

//==============================================================================
//Global functions

void open_empty_hex(int i, int j) {
	for (int x = -1; x < 2; ++x) {
		for (int y = -1; y < 2; ++y) {
			if (i + x > -1 && i + x < m && j + y > -1 && j + y < n && !(x == 0 && y == 0)) {
				if (open[i + x][j + y] == 0) {
					open[i + x][j + y] = 1;
					if (pole[i + x][j + y] == 0) {
						open_empty_hex(i + x, j + y);
					}
				}
			}
		}
	}
}

void first_bomb(vector<vector<int>>& pole, vector<vector<int>>& open, int& x, int& y) {
	std::vector<pair<int,int>> random;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			bool new_mine = 1;
			for (int i1 = -1; i1 < 2; ++i1) {
				for (int j1 = -1; j1 < 2; ++j1) {
					if (i + i1 > -1 && i + i1 < m && j + j1 > -1 && j + j1 < n && !(i1 == 0 && j1 == 0)) {
						if (open[i + i1][j + j1] == 1) {
							new_mine = 0;
						}
					}
				}
			}
			if (new_mine && !(i == x && j == y)) {
				random.push_back(std::pair<int, int>(i, j));
			}
		}
	}
	pole[x][y] = 0;
	open[x][y] = 1;
	int random_index = rand() % random.size();
	int x1 = random[random_index].first;
	int y1 = random[random_index].second;
	pole[x1][y1] = -1;
	for (int i = -1; i < 2; ++i) {
		for (int j = -1; j < 2; ++j) {
			if (i + x > -1 && i + x < m && j + y > -1 && j + y < n && !(i == 0 && j == 0)) {
				if (pole[x + i][y + j] != -1) {
					pole[x + i][y + j] -= 1;
				}
				if (pole[i + x][j + y] == -1) {
					pole[x][y] += 1;
				}
			}
			if (i + x1 > -1 && i + x1 < m && j + y1 > -1 && j + y1 < n && !(i == 0 && j == 0)) {
				if (pole[x1 + i][y1 + j] != -1) {
					pole[x1 + i][y1 + j] += 1;
				}
			}
		}
	}
	if (pole[x][y] == 0) {
		open_empty_hex(x, y);
	}
}

int fact_situation (int i, int j) {
	bool There_hex_mine = 1;
	open[i][j] = 3;
	for (int i1 = -1; i1 < 2; ++i1) {
		for (int j1 = -1; j1 < 2; ++j1) {
			if (i1 + i > -1 && i1 + i < m && j1 + j > -1 && j1 + j < n && !(i1 == 0 && j1 == 0)) {
				if (AI_pole[i1 + i][j1 + j] != -1) {
					AI_pole[i1 + i][j1 + j] -= 1;
					if (AI_pole[i1 + i][j1 + j] == -1) {
						AI_pole[i1 + i][j1 + j] = -3;
					}
				}
			}
		}
	}
	int There_null = 0;
	while (There_hex_mine || There_null) {
	There_null = 0;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			if (AI_pole[i][j] == 0 && open[i][j] == 1) {
				for (int i1 = -1; i1 < 2; ++i1) {
					for (int j1 = -1; j1 < 2; ++j1) {
						if (i1 + i > -1 && i1 + i < m && j1 + j > -1 && j1 + j < n && !(i1 == 0 && j1 == 0)) {
							if (open[i + i1][j + j1] == 0) {
								There_null = 1;
								open[i + i1][j + j1] = 4;
							}
						}
					}
				}
			}
		}
	}
	There_hex_mine = 0;
	int num = 0;
	while (!There_hex_mine && num < n * m) {
		int xc = num / m;
		int yc = num % m;
		if (open[xc][yc] == 1 && AI_pole[xc][yc] != 0) {
			int Count_mine = 0;
			for (int i = -1; i < 2; ++i) {
				for (int j = -1; j < 2; ++j) {
					if (i + xc > -1 && i + xc < m && j + yc > -1 && j + yc < n && !(i == 0 && j == 0)) {
						if (open[i + xc][j + yc] == 0) {
							Count_mine += 1;
						}
					}
				}
			}
			if (Count_mine == AI_pole[xc][yc]) {
				There_hex_mine = 1;
				for (int i = -1; i < 2; ++i) {
					for (int j = -1; j < 2; ++j) {
						if (i + xc > -1 && i + xc < m && j + yc > -1 && j + yc < n && !(i == 0 && j == 0)) {
							if (open[i + xc][j + yc] == 0) {
								open[i + xc][j + yc] = 3;
								for (int i1 = -1; i1 < 2; ++i1) {
									for (int j1 = -1; j1 < 2; ++j1) {
										if (i1 + i + xc > -1 && i1 + i + xc < m && j1 + j + yc > -1 && j1 + j + yc < n && !(i1 == 0 && j1 == 0)) {
											if (AI_pole[i1 + i + xc][j1 + j + yc] != -1) {
												AI_pole[i1 + i + xc][j1 + j + yc] -= 1;
												if (AI_pole[i1 + i + xc][j1 + j + yc] == -1) {
													AI_pole[i1 + i + xc][j1 + j + yc] = -3;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			for (int i = 0; i < m; ++i) {
				for (int j = 0; j < n; ++j) {
					Count_mine = 0;
					if (open[i][j] == 1 && AI_pole[i][j] > 0) {
						for (int i1 = -1; i1 < 2; ++i1) {
							for (int j1 = -1; j1 < 2; ++j1) {
								if (i1 + i > -1 && i1 + i < m && j1 + j > -1 && j1 + j < n && !(i1 == 0 && j1 == 0)) {
									if (open[i1 + i][j1 + j] == 0) {
										Count_mine += 1;
									}
								}
							}
						}
					}
					if (AI_pole[i][j] < -2 && open[i][j] == 1 || AI_pole[i][j] > Count_mine && AI_pole[i][j] > 0 && open[i][j] == 1) {
						for (int xc = 0; xc < m; ++xc) {
							for (int yc = 0; yc < n; ++yc) {
								if (open[xc][yc] == 3) {
									open[xc][yc] = 0;
									for (int i1 = -1; i1 < 2; ++i1) {
										for (int j1 = -1; j1 < 2; ++j1) {
											if (i1 + xc > -1 && i1 + xc < m && j1 + yc > -1 && j1 + yc < n && !(i1 == 0 && j1 == 0)) {
												if (AI_pole[i1 + xc][j1 + yc] != -1) {
													if (AI_pole[i1 + xc][j1 + yc] == -3) {
														AI_pole[i1 + xc][j1 + yc] = 0;
													}
													AI_pole[i1 + xc][j1 + yc] += 1;
												}
											}
										}
									}
								}
								if (open[xc][yc] == 4) {
									open[xc][yc] = 0;
								}
							}
						}
						return 0;
					}
				}
			}
		}
		++num;
	}
	}
	for (int xc = 0; xc < m; ++xc) {
		for (int yc = 0; yc < n; ++yc) {
			if (open[xc][yc] == 3) {
				open[xc][yc] = 0;
				for (int i1 = -1; i1 < 2; ++i1) {
					for (int j1 = -1; j1 < 2; ++j1) {
						if (i1 + xc > -1 && i1 + xc < m && j1 + yc > -1 && j1 + yc < n && !(i1 == 0 && j1 == 0)) {
							if (AI_pole[i1 + xc][j1 + yc] != -1) {
								if (AI_pole[i1 + xc][j1 + yc] == -3) {
									AI_pole[i1 + xc][j1 + yc] = 0;
								}
								AI_pole[i1 + xc][j1 + yc] += 1;
							}
						}
					}
				}
			}
			if (open[xc][yc] == 4) {
				open[xc][yc] = 0;
			}
		}
	}
	return 1;
}

//==============================================================================
//CMainWin

class CMainWin : public CFrameWnd
{
public:
	CMainWin(CString Title, int HowShow = SW_RESTORE);
	afx_msg void OnPaint();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT flags, CPoint Loc);
	afx_msg void OnRButtonDown(UINT flags, CPoint Loc);
	afx_msg void OnLButtonDblClk(UINT flags, CPoint Loc);
	void Menu_NewGame();
	//void Menu_Options();
	void Menu_About();
	void Menu_Exit();
	void Menu_AI_Game();
	void AI_Change();
	int x, y;
private:
	CBitmap *HexBitmap;
	CBitmap *Hex_vis[14];
	CMenu m_wndMenu;
	DECLARE_MESSAGE_MAP();
};
BEGIN_MESSAGE_MAP(CMainWin, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP(NEW_MENU, mfc_var)	
	ON_WM_RBUTTONDOWN(NEW_MENU, mfc_var)
	ON_WM_LBUTTONDBLCLK(NEW_MENU, mfc_var)
	ON_COMMAND(ID_FILE_NEWGAME, Menu_NewGame)
	//ON_COMMAND(ID_FILE_OPTIONS, Menu_Options)
	ON_COMMAND(ID_HELP_ABOUT, Menu_About)
	ON_COMMAND(ID_FILE_EXIT, Menu_Exit)
	ON_COMMAND(ID_SPECIAL_AIGAME, Menu_AI_Game)
	//ON_COMMAND(ID_HELP_ABOUT, Menu_About)
END_MESSAGE_MAP()

int CMainWin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)  return -1;
	m_wndMenu.LoadMenu(130);
	SetMenu(&m_wndMenu);
	return 0;
}

class CApp : public CWinApp
{
public:
	BOOL InitInstance();
};
CApp App;

CMainWin::CMainWin(CString Title, int HowShow) {
 	RECT rect;
 	rect.top = 0;
 	rect.left = 0;
 	rect.right = 50 * m + 55;
 	rect.bottom = 200 + 50 * n;
 	this->Create(0, Title, 
                         WS_OVERLAPPED | WS_CAPTION | WS_DLGFRAME 
                         | WS_SYSMENU | WS_MINIMIZEBOX, rect, 0, 0);
 	this->CenterWindow();
 	this->ShowWindow(HowShow);
	this->UpdateWindow();
	Menu_NewGame();
}

//==============================================================================
//AI_Change

void CMainWin::AI_Change() {
	bool There_hex_mine = 0;
	x = -1;
	y = -1;
	int num = 0;
	while (!There_hex_mine && num < n * m) {
		int xc = num / m;
		int yc = num % m;
		if (open[xc][yc] == 1 && AI_pole[xc][yc] != 0) {
			int Count_mine = 0;
			for (int i = -1; i < 2; ++i) {
				for (int j = -1; j < 2; ++j) {
					if (i + xc > -1 && i + xc < m && j + yc > -1 && j + yc < n && !(i == 0 && j == 0)) {
						if (open[i + xc][j + yc] == 0) {
							Count_mine += 1;
						}
					}
				}
			}
			if (Count_mine == AI_pole[xc][yc]) {
				There_hex_mine = 1;
				for (int i = -1; i < 2; ++i) {
					for (int j = -1; j < 2; ++j) {
						if (i + xc > -1 && i + xc < m && j + yc > -1 && j + yc < n && !(i == 0 && j == 0)) {
							if (open[i + xc][j + yc] == 0) {
								open[i + xc][j + yc] = 2;
								this->InvalidateRect(0);
								for (int i1 = -1; i1 < 2; ++i1) {
									for (int j1 = -1; j1 < 2; ++j1) {
										if (i1 + i + xc > -1 && i1 + i + xc < m && j1 + j + yc > -1 && j1 + j + yc < n && !(i1 == 0 && j1 == 0)) {
											if (AI_pole[i1 + i + xc][j1 + j + yc] != -1) {
												AI_pole[i1 + i + xc][j1 + j + yc] -= 1;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		++num;
	}
	bool There_open_hex = 0;
	if (!There_hex_mine) {
		num = 0;
		int xc = 0;
		int yc = 0;
		bool There_open = 0;
		while ((AI_pole[xc][yc] != 0 || There_open == 0) && num < n * m) {
			xc = num / m;
			yc = num % m;
			There_open = 0;
			if (AI_pole[xc][yc] == 0 && open[xc][yc] == 1) {
				There_open_hex = 1;
				for (int i = -1; i < 2; ++i) {
					for (int j = -1; j < 2; ++j) {
						if (i + xc > -1 && i + xc < m && j + yc > -1 && j + yc < n && !(i == 0 && j == 0)) {
							if (open[i + xc][j + yc] == 0) {
								There_open = 1;
								x = i + xc;
								y = j + yc;
								this->InvalidateRect(0);
							}
						}
					}
				}
			}
			++num;
		}
	}
	/*if (!There_open_hex && !There_hex_mine) {
		num = 0;
		int xc = 0;
		int yc = 0;
		bool There_open = 0;
		while ((AI_pole[xc][yc] != 0 || There_open == 0) && num < n * m) {

		}
	}*/
}

//==============================================================================
//Open hex

afx_msg void CMainWin::OnPaint() {
	HexBitmap=new CBitmap();
	HexBitmap->LoadBitmap(101);
	CPaintDC clientDC(this);
	CDC memDC;
 	memDC.CreateCompatibleDC(&clientDC);
	for (int i = 0; i < 15; ++i) {
		Hex_vis[i]=new CBitmap();
		Hex_vis[i]->LoadBitmap(103 + i);
		CPaintDC clientDC(this);
		CDC memDC;
 		memDC.CreateCompatibleDC(&clientDC);
	}
	if (endgame == 0) {
		memDC.SelectObject(Hex_vis[11]);
		BITMAP bmp;
		HexBitmap->GetBitmap(&bmp);
		clientDC.BitBlt(25 * m - 18, 20, 90, 90, &memDC, 0, 0, SRCCOPY);
	} else if (endgame == 1) {
		memDC.SelectObject(Hex_vis[12]);
		BITMAP bmp;
		HexBitmap->GetBitmap(&bmp);
		clientDC.BitBlt(25 * m - 18, 20, 90, 90, &memDC, 0, 0, SRCCOPY);
	} else {
		memDC.SelectObject(Hex_vis[13]);
		BITMAP bmp;
		HexBitmap->GetBitmap(&bmp);
		clientDC.BitBlt(25 * m - 18, 20, 90, 90, &memDC, 0, 0, SRCCOPY);
	}
	if (!endgame) {
		if(x > -1 && x < m && y > -1 && y < n) {
			if (open[x][y] != 2) {
				if (first_step && pole[x][y] == -1) {
					first_step = 0;
					first_bomb(pole, open, x, y);
				} else {
					first_step = 0;
					open[x][y] = 1;
					if (pole[x][y] == -1) {
						int fact_sit = 0;
						open[x][y] = 0;
						for (int i = -1; i < 2; ++i) {
							for (int j = -1; j < 2; ++j) {
								if (i + x > -1 && i + x < m && j + y > -1 && j + y < n && !(i == 0 && j == 0)) {
									if (open[i + x][j + y] == 0 && pole[i + x][j + y] != -1) {
										int f = fact_situation(i + x, j + y);
										fact_sit += f;
									}
								}
							}
						}
						if (fact_sit && !dblclk) {
							open[x][y] = 2;
							for (int i = -1; i < 2; ++i) {
								for (int j = -1; j < 2; ++j) {
									if (i + x > -1 && i + x < m && j + y > -1 && j + y < n && !(i == 0 && j == 0)) {
										if (AI_pole[i + x][j + y] != -1) {
											AI_pole[i + x][j + y] -= 1;
										}
									}
								}
							}
						} else {
							endgame = 1;
							memDC.SelectObject(Hex_vis[12]);
							BITMAP bmp;
							HexBitmap->GetBitmap(&bmp);
							clientDC.BitBlt(25 * m - 18, 20, 90, 90, &memDC, 0, 0, SRCCOPY);
							for (int i = 0; i < m; ++i) {
								for (int j = 0; j < n; ++j) {
									if (pole[i][j] == -1 && open[i][j] == 0) {
										open[i][j] = 1;
									} else if (pole[i][j] != -1 && open[i][j] == 2) {
										open[i][j] = 3;
									}
								}
							}
						}
					} else if (pole[x][y] == 0){
						open_empty_hex(x, y);
					}
				}
			}
		}
	}
	dblclk = 0;
	int open_hex_count = 0;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			if (open[i][j] == 1) {
				memDC.SelectObject(Hex_vis[(pole[i][j] == -1) ? 9 : pole[i][j]]);
				BITMAP bmp;
				Hex_vis[(pole[i][j] == -1) ? 9 : pole[i][j]]->GetBitmap(&bmp);
				clientDC.BitBlt(i * 50 + 25, j * 50 + 125, bmp.bmWidth, bmp.bmHeight, &memDC, 0, 0, SRCCOPY);
				open_hex_count += 1;
			} else if (open[i][j] == 0) {
				memDC.SelectObject(HexBitmap);
				BITMAP bmp;
				HexBitmap->GetBitmap(&bmp);
				clientDC.BitBlt(i * 50 + 25, j * 50 + 125, bmp.bmWidth, bmp.bmHeight, &memDC, 0, 0, SRCCOPY);
			} else if (open[i][j] == 2) {
				memDC.SelectObject(Hex_vis[10]);
				BITMAP bmp;
				HexBitmap->GetBitmap(&bmp);
				clientDC.BitBlt(i * 50 + 25, j * 50 + 125, bmp.bmWidth, bmp.bmHeight, &memDC, 0, 0, SRCCOPY);
			} else {
				memDC.SelectObject(Hex_vis[14]);
				BITMAP bmp;
				HexBitmap->GetBitmap(&bmp);
				clientDC.BitBlt(i * 50 + 25, j * 50 + 125, bmp.bmWidth, bmp.bmHeight, &memDC, 0, 0, SRCCOPY);
			}
		}
	}
	if (open_hex_count == m * n - max_bomb && !endgame) {
		endgame = 2;
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				if (open[i][j] == 0) {
					open[i][j] = 2;
				}
			}
		}
		this->InvalidateRect(0);
	}
}

void CMainWin::OnLButtonUp(UINT flags, CPoint Loc)
{
	x = (Loc.x - 25) / 50;
	y = (Loc.y - 125) / 50;
	this->InvalidateRect(0);
}

void CMainWin::OnRButtonDown(UINT flags, CPoint Loc)
{
	x = (Loc.x - 25) / 50;
	y = (Loc.y - 125) / 50;
	if(!(endgame) && x > -1 && x < m && y > -1 && y < n) {
		open[x][y] = 2 - open[x][y];
	}
	for (int i1 = -1; i1 < 2; ++i1) {
		for (int j1 = -1; j1 < 2; ++j1) {
			if (i1 + x > -1 && i1 + x < m && j1 + y > -1 && j1 + y < n && !(i1 == 0 && j1 == 0)) {
				if (AI_pole[i1 + x][j1 + y] != -1) {
					AI_pole[i1 + x][j1 + y] -= (open[x][y] == 2) ? 1 : -1;
				}
			}
		}
	}
	x = -1;
	y = -1;
	this->InvalidateRect(0);
}

void CMainWin::OnLButtonDblClk(UINT flags, CPoint Loc)
{
	dblclk = 1;
	int x1 = (Loc.x - 25) / 50;
	int y1 = (Loc.y - 125) / 50;
	if (open[x1][y1] == 1) {
		int Count_flag = 0;
		for (int i = -1; i < 2; ++i) {
			for (int j = -1; j < 2; ++j) {
				if (i + x1 > -1 && i + x1 < m && j + y1 > -1 && j + y1 < n && !(i == 0 && j == 0)) {
					if (open[i + x1][j + y1] == 2) {
						Count_flag += 1;
					}
				}
			}
		}
		if (Count_flag == pole[x1][y1]) {
			for (int i = -1; i < 2; ++i) {
				for (int j = -1; j < 2; ++j) {
					if (i + x1 > -1 && i + x1 < m && j + y1 > -1 && j + y1 < n && !(i == 0 && j == 0)) {
						if (open[i + x1][j + y1] == 0) {
							if (pole[x1 + i][y1 + j] == 0) {
								open_empty_hex(x1 + i, y1 + j);
								//this->InvalidateRect(0);
							}
							if (pole[x1 + i][y1 + j] == -1) {
								x = i + x1;
								y = j + y1;
								this->InvalidateRect(0);
							}
							open[x1 + i][y1 + j] = 1;
						}
					}
				}
			}
		}
	}
	this->InvalidateRect(0);
}

//==============================================================================
//New game

void CMainWin::Menu_NewGame() {
	first_step = 1;
	endgame = 0;
	bomb = 0;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			pole[i][j] = 0;
			open[i][j] = 0;
		}
	}
	srand(time(NULL));
	while (bomb < max_bomb) {
		int x = rand() % m;
		int y = rand() % n;
		if (pole[x][y] != -1) {
			pole[x][y] = -1;
			bomb += 1;
		}
	}
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			for (int x = -1; x < 2; ++x) {
				for (int y = -1; y < 2; ++y) {
					if (i + x > -1 && i + x < m && j + y > -1 && j + y < n && !(x == 0 && y == 0)) {
						if (pole[i + x][j + y] == -1 && pole[i][j] != -1) {
							pole[i][j] = pole[i][j] + 1;
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			AI_pole[i][j] = pole[i][j];
			if (pole[i][j] == 0) {
				AI_pole[i][j] = -2;
			}
		}
	}
	x = -1;
	y = -1;
	this->InvalidateRect(0);
}

//==============================================================================

/*void CMainWin::Menu_Options() {
	
}*/

void CMainWin::Menu_About() {
	CDialog about(128);
	int a = about.DoModal();
}

void CMainWin::Menu_Exit() {
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}

void CMainWin::Menu_AI_Game() {
	AI_Change();
}

BOOL CApp::InitInstance()
 	{
 	m_pMainWnd = new CMainWin("saper+",
                                   SW_RESTORE); 
	ASSERT(m_pMainWnd);
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
 	return TRUE;
 	}