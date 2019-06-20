#ifndef CARD_H
#define CARD_H

#include "Inside.h"

void Card_Init();
void ViewCard(int, int,int, int);
void GraphDraw(int, status *p, status *d);
void Text(int, status *p, status *d);
void MoveCard(int, int, int, int, int);
//void AnimeStart();
void EndText(status, status);
int ContinueText();

#endif