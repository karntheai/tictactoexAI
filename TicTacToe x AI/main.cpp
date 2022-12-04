#include <iostream>
#include <raylib.h>
using std::string;
using std::to_string;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int k = 10; // thickness of grid bars
const Color OCol = {255, 51, 153, 255};
const Color XCol = {102, 255, 153, 255};
const Color BG = {0, 0, 26, 255};
const Color GRID = {128, 229, 255, 255};
const string title = "Tic Tac Toe";

int wlno = 0;
class Board
{
  int Cells[3][3];
  float ci, cj;

  int available = 9;

public:
  int winner = 0;
  bool move =false;
  bool newGame = true;
  int Player = 1;
  Board()
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        Cells[i][j] = 0;
  }
  void DrawX(int i, int j)
  {

    float Xlen = (SCREEN_WIDTH - 10) / 6 - 55;

    float xi = 5 + (SCREEN_WIDTH - 10) / 6 + j * (SCREEN_WIDTH - 10) / 3;
    float xj = 5 + (SCREEN_HEIGHT - 10) / 6 + i * (SCREEN_HEIGHT - 10) / 3;
    DrawLineEx({xi - Xlen, xj - Xlen}, {xi + Xlen, xj + Xlen}, 12, XCol);
    DrawLineEx({xi - Xlen, xj + Xlen}, {xi + Xlen, xj - Xlen}, 12, XCol);
  }
  void Update()
  {move = false;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && newGame && Player == 1 && available != 0)
    {

      Vector2 mPos = GetMousePosition();
      for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
          if ((mPos.x > 5 + (j * (SCREEN_WIDTH - 10) / 3) && mPos.x < 5 + ((j + 1) * (SCREEN_WIDTH - 10) / 3)) && (mPos.y > 5 + (i * (SCREEN_HEIGHT - 10) / 3)) && mPos.y < 5 + ((i + 1) * (SCREEN_HEIGHT - 10) / 3))
          {
            if (Cells[i][j] == 0)
            { move = true;
              Cells[i][j] = Player, Player = changePlayer(Player);
              ci = i, cj = j;
              --available;
            }
          }
    }
    else if (Player == 2 && newGame && available != 0)
    {
      int emptyCells[available][3];
      int cnt = 0;
      int wl1 = 0, wl2 = 0, ll = 0;
      for (int i = 0; i < 3; i++)
      {
        for (int j = 0; j < 3; j++)
          if (Cells[i][j] == 0)
          {
            emptyCells[cnt][0] = i;
            emptyCells[cnt][1] = j;

            Cells[i][j] = 2;
            if (win({(float)i, (float)j}) == 1)
            {
              ++wl1;
              emptyCells[cnt][2] = -15;
            }
            else
            {
              Cells[i][j] = 1;
              if (win({(float)i, (float)j}) == 1)
              {
                emptyCells[cnt][2] = -10;
                ++wl2;
              }
              else
                emptyCells[cnt][2] = 0, ++ll;
            }
            Cells[i][j] = 0;
            ++cnt;
          }
      }

      int w1Cell[wl1][2], w2Cell[wl2][2], lCell[ll][2];
      wl1 = 0, wl2 = 0;
      ll = 0;
      for (int i = 0; i < available; i++)
      {
        if (emptyCells[i][2] == -15)
        {
          w1Cell[wl1][0] = emptyCells[i][0];
          w1Cell[wl1][1] = emptyCells[i][1];
          ++wl1;
        }
        else if (emptyCells[i][2] == -10)
        {
          w2Cell[wl2][0] = emptyCells[i][0];
          w2Cell[wl2][1] = emptyCells[i][1];
          ++wl2;
        }
        else
        {
          lCell[ll][0] = emptyCells[i][0];
          lCell[ll][1] = emptyCells[i][1];
          ++ll;
        }
      }

      if (wl1 > 0)
      {
        int temp = GetRandomValue(0, wl1-1);
        Cells[w1Cell[temp][0]][w1Cell[temp][1]] = 2;
        ci = w1Cell[temp][0], cj = w1Cell[temp][1];
      }
      else if (wl2 > 0)
      {
        int temp = GetRandomValue(0, wl2-1);
        Cells[w2Cell[temp][0]][w2Cell[temp][1]] = 2;
        ci = w2Cell[temp][0], cj = w2Cell[temp][1];
      }
      else
      {
        int temp = GetRandomValue(0, ll-1);
        Cells[lCell[temp][0]][lCell[temp][1]] = 2;
        ci = lCell[temp][0], cj = lCell[temp][1];
      }

      Player = changePlayer(Player);
      --available;
      // printf("Win1: %d, Win2: %d, Lose: %d, Available: %d\n", wl1, wl2, ll, available + 1);
    }
    winner = 3;
    }
  void draw()
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
      {
        Vector2 pos = {(float)(5 + SCREEN_WIDTH / 6) + j * (SCREEN_WIDTH / 3), (float)(5 + SCREEN_HEIGHT / 6 + (SCREEN_HEIGHT / 3) * i)};
        if (Cells[i][j] == 1)
        {
          // DrawPoly(pos, 3, 8 + SCREEN_WIDTH / 12, 180, XCol);
          // DrawPoly(pos, 3, -8 + SCREEN_WIDTH / 12, 180, BG);
          DrawX(i, j);
        }
        else if (Cells[i][j] == 2)
        {
          DrawCircleV(pos, 6 + SCREEN_WIDTH / 15, OCol);
          DrawCircleV(pos, -6 + SCREEN_WIDTH / 15, BG);
        }
      }

    if (win({ci, cj}) && win({ci, cj}) != 3 && available != 9)
    {
      newGame = false;
      string whoWon;
      DrawRectangle(-150 + SCREEN_WIDTH / 2, -50 + SCREEN_HEIGHT / 2, 300, 100, Fade(YELLOW, 0.8f));
      if (win({ci, cj}) == 1){
        whoWon = "\t\tYOU WON!";
      winner = 1;
        }
      else{winner = 2;
        whoWon = "\t\t AI WON!";}

      DrawText(whoWon.c_str(), -130 + SCREEN_WIDTH / 2, -20 + SCREEN_HEIGHT / 2, 40, Fade(BLACK, 0.8f));
    }
    else if (win({ci, cj}) == 3 && available != 9)
    {
      newGame = false;
      DrawRectangle(-150 + SCREEN_WIDTH / 2, -50 + SCREEN_HEIGHT / 2, 300, 100, Fade(YELLOW, 0.8f));
      DrawText("Tie!", -40 + SCREEN_WIDTH / 2, -20 + SCREEN_HEIGHT / 2, 40, Fade(BLACK, 0.8f));
    }
  }
  void reset()
  {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        Cells[i][j] = 0;
    available = 9;
    Player = 1;
    newGame = true;
    winner = 0;
    wlno = 0;
  }
  int win(Vector2 cell)
  {
    if (cell.x == 0 && Cells[(int)cell.x, (int)cell.y] != 0)
    {
      switch ((int)cell.y)
      {
      case 0:
      {
        if (Cells[0][0] == Cells[0][1] && Cells[0][2] == Cells[0][0])
          return changePlayer(Player);
        else if (Cells[0][0] == Cells[1][0] && Cells[2][0] == Cells[0][0])
          return changePlayer(Player);
        else if (Cells[0][0] == Cells[1][1] && Cells[2][2] == Cells[0][0])
          return changePlayer(Player);
      }
      break;
      case 1:
      {
        if (Cells[0][1] == Cells[0][0] && Cells[0][2] == Cells[0][1])
          return changePlayer(Player);
        else if (Cells[0][1] == Cells[1][1] && Cells[2][1] == Cells[0][1])
          return changePlayer(Player);
      }
      break;
      case 2:
      {
        if (Cells[0][2] == Cells[0][1] && Cells[0][2] == Cells[0][0])
          return changePlayer(Player);
        else if (Cells[0][2] == Cells[1][2] && Cells[2][2] == Cells[0][2])
          return changePlayer(Player);
        else if (Cells[0][2] == Cells[1][1] && Cells[2][0] == Cells[0][2])
          return changePlayer(Player);
      }
      break;
      }
    }
    else if (cell.x == 1 && Cells[(int)cell.x, (int)cell.y] != 0)
    {
      switch ((int)cell.y)
      {
      case 0:
      {
        if (Cells[1][0] == Cells[0][0] && Cells[2][0] == Cells[1][0])
          return changePlayer(Player);
        else if (Cells[1][0] == Cells[1][1] && Cells[1][2] == Cells[1][0])
          return changePlayer(Player);
      }
      break;
      case 1:
      {
        if (Cells[1][1] == Cells[0][1] && Cells[2][1] == Cells[1][1])
          return changePlayer(Player);
        else if (Cells[1][1] == Cells[0][0] && Cells[1][1] == Cells[2][2])
          return changePlayer(Player);
        else if (Cells[1][0] == Cells[1][1] && Cells[1][1] == Cells[1][2])
          return changePlayer(Player);
        else if (Cells[2][0] == Cells[1][1] && Cells[1][1] == Cells[0][2])
          return changePlayer(Player);
      }
      break;
      case 2:
      {

        if (Cells[1][2] == Cells[0][2] && Cells[0][2] == Cells[2][2])
          return changePlayer(Player);
        else if (Cells[1][2] == Cells[1][1] && Cells[1][0] == Cells[1][2])
          return changePlayer(Player);
      }
      break;
      }
    }
    else if (cell.x == 2 && Cells[(int)cell.x, (int)cell.y] != 0)
    {
      switch ((int)cell.y)
      {
      case 0:
      {
        if (Cells[2][0] == Cells[2][1] && Cells[2][2] == Cells[0][0])
          return changePlayer(Player);
        else if (Cells[2][0] == Cells[1][0] && Cells[2][0] == Cells[0][0])
          return changePlayer(Player);
        else if (Cells[2][0] == Cells[1][1] && Cells[2][0] == Cells[0][2])
          return changePlayer(Player);
      }
      break;
      case 1:
      {
        if (Cells[2][1] == Cells[2][0] && Cells[2][1] == Cells[2][2])
          return changePlayer(Player);
        else if (Cells[2][1] == Cells[1][1] && Cells[2][1] == Cells[0][1])
          return changePlayer(Player);
      }
      break;
      case 2:
      {
        if (Cells[2][2] == Cells[2][1] && Cells[2][2] == Cells[2][0])
          return changePlayer(Player);
        else if (Cells[2][2] == Cells[1][2] && Cells[2][2] == Cells[0][2])
          return changePlayer(Player);
        else if (Cells[2][2] == Cells[1][1] && Cells[2][2] == Cells[0][0])
          return changePlayer(Player);
      }
      break;
      }
    }
    if (available == 0)
      return 3;
    return 0;
  }
  int changePlayer(int plyr)
  {
    return (plyr == 1) ? 2 : 1;
  }
};

void drawGrid(int l, int h, Color col)
{
  float x = l / 3;
  float y = h / 3;

  Color col1 = {0, 255, 205, 255};
  DrawRectangleRounded({5, y - k / 2, float(l - 10), k}, 1, 10, col1);
  DrawRectangleRounded({5, 2 * y - k / 2, float(l - 10), k}, 1, 10, col1);
  DrawRectangleRounded({x - k / 2, 5, k, float(h - 10)}, 1, 10, col1);
  DrawRectangleRounded({2 * x - k / 2, 5, k, float(h - 10)}, 1, 10, col1);
  DrawRectangleRounded({5, y - k / 3, float(l - 10), k / 2}, 1, 10, col);
  DrawRectangleRounded({5, 2 * y - k / 3, float(l - 10), k / 2}, 1, 10, col);
  DrawRectangleRounded({x - k / 3, 5, k / 2, float(h - 10)}, 1, 10, col);
  DrawRectangleRounded({2 * x - k / 3, 5, k / 2, float(h - 10)}, 1, 10, col);
  // for(int i = 0;i < 3; i++){
  //   for(int j = 0; j < 3; j++)
  //   DrawText(("[" + to_string(i)+","+to_string(j)+"]").c_str(),20+((SCREEN_WIDTH-10)/3)*j,20+((SCREEN_HEIGHT-10)/3)*i,15,GRAY);
  // }
}

int main()
{
  Board myboard;
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, title.c_str());
  SetTargetFPS(10);
  string Player;

InitAudioDevice();
Sound move = LoadSound("src/move.wav");
Sound aiMove = LoadSound("src/aimove.wav");
Sound lose = LoadSound("src/lose.wav");
Sound win = LoadSound("src/win.wav");
Sound thinking = LoadSound("src/thinking.wav");
SetSoundPitch(thinking,0.5f);
SetSoundVolume(thinking,0.5f);
  int frameCount = 0, t = 12, Cno = 1;
  while (!WindowShouldClose())
  {
    if (IsKeyPressed(KEY_R))
      myboard.reset();

    BeginDrawing();
    ClearBackground(BG);
    if (myboard.Player == 2 && myboard.newGame)
    {
      frameCount++;
      float alpha;
      if ((frameCount / 3) % 2)
        alpha = 0.0f;
      else
        alpha = 0.8f;
      if (Cno < t)
        DrawRectangle(SCREEN_WIDTH - 105, 18, 22, 22, Fade({0, 255, 255, 255}, alpha)), Cno++;
      else
        Cno = 1;
      if(frameCount%4==0)
      PlaySound(thinking);
      if (frameCount == t)
      {  StopSound(thinking);
        myboard.Update();
        PlaySound(aiMove);
        frameCount = 0;
      }

    }
    else{
      myboard.Update();
      if(myboard.move)
          PlaySound(move);
      }
    if (myboard.Player == 1)
      Player = "YOU";
    else
      Player = "AI ";
    if (myboard.newGame)
      DrawText(Player.c_str(), SCREEN_WIDTH - 70, 15, 30, Fade(WHITE, 0.8f));
    drawGrid(SCREEN_WIDTH, SCREEN_HEIGHT, GRID);
    myboard.draw();
  if(myboard.winner == 1 )
     {
      
       if(wlno < 10)
{      PlaySound(win);
wlno+= 4;
}    
     }
  else if(myboard.winner == 2){
    
   if(wlno < 10){
    PlaySound(lose);
    wlno += 5;}
   
     
  }
    EndDrawing();
  }
  UnloadSound( aiMove);
  UnloadSound(move);
  UnloadSound(win);
  UnloadSound(lose);
  UnloadSound(thinking);
CloseAudioDevice();


  return 0;
}