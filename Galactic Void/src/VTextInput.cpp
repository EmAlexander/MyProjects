/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#include "VTextInput.h"


VTextInput::VTextInput()
{
}


VTextInput::~VTextInput()
{
}

void VTextInput::handleEvents(SDL_Event* e, unsigned int limit)
{
	if (e->type == SDL_KEYUP)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_LSHIFT:
			shiftDown = false;
			break;
		case SDLK_RSHIFT:
			shiftDown = false;
			break;
		}
	}
	else if (e->type == SDL_KEYDOWN && !shiftDown)
	{
		if (e->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
		{
			string inputText = SDL_GetClipboardText();

			if (inputText.length() + text.length() <= limit)
			{
				text += inputText;
			}
		}

		switch (e->key.keysym.sym)
		{
		case SDLK_1:
			if (text.length() < limit)
				text += "1";
			break;
		case SDLK_2:
			if (text.length() < limit)
				text += "2";
			break;
		case SDLK_3:
			if (text.length() < limit)
				text += "3";
			break;
		case SDLK_4:
			if (text.length() < limit)
				text += "4";
			break;
		case SDLK_5:
			if (text.length() < limit)
				text += "5";
			break;
		case SDLK_6:
			if (text.length() < limit)
				text += "6";
			break;
		case SDLK_7:
			if (text.length() < limit)
				text += "7";
			break;
		case SDLK_8:
			if (text.length() < limit)
				text += "8";
			break;
		case SDLK_9:
			if (text.length() < limit)
				text += "9";
			break;
		case SDLK_0:
			if (text.length() < limit)
				text += "0";
			break;
		case SDLK_q:
			if (text.length() < limit)
				text += "q";
			break;
		case SDLK_w:
			if (text.length() < limit)
				text += "w";
			break;
		case SDLK_e:
			if (text.length() < limit)
				text += "e";
			break;
		case SDLK_r:
			if (text.length() < limit)
				text += "r";
			break;
		case SDLK_t:
			if (text.length() < limit)
				text += "t";
			break;
		case SDLK_y:
			if (text.length() < limit)
				text += "y";
			break;
		case SDLK_u:
			if (text.length() < limit)
				text += "u";
			break;
		case SDLK_i:
			if (text.length() < limit)
				text += "i";
			break;
		case SDLK_o:
			if (text.length() < limit)
				text += "o";
			break;
		case SDLK_p:
			if (text.length() < limit)
				text += "p";
			break;
		case SDLK_LEFTBRACKET:
			if (text.length() < limit)
				text += "[";
			break;
		case SDLK_RIGHTBRACKET:
			if (text.length() < limit)
				text += "]";
			break;
		case SDLK_BACKSLASH:
			if (text.length() < limit)
				text += "\\";
			break;
		case SDLK_BACKQUOTE:
			if (text.length() < limit)
				text += "`";
			break;
		case SDLK_EQUALS:
			if (text.length() < limit)
				text += "=";
			break;
		case SDLK_MINUS:
			if (text.length() < limit)
				text += "-";
			break;
		case SDLK_a:
			if (text.length() < limit)
				text += "a";
			break;
		case SDLK_s:
			if (text.length() < limit)
				text += "s";
			break;
		case SDLK_d:
			if (text.length() < limit)
				text += "d";
			break;
		case SDLK_f:
			if (text.length() < limit)
				text += "f";
			break;
		case SDLK_g:
			if (text.length() < limit)
				text += "g";
			break;
		case SDLK_h:
			if (text.length() < limit)
				text += "h";
			break;
		case SDLK_j:
			if (text.length() < limit)
				text += "j";
			break;
		case SDLK_k:
			if (text.length() < limit)
				text += "k";
			break;
		case SDLK_l:
			if (text.length() < limit)
				text += "l";
			break;
		case SDLK_SEMICOLON:
			if (text.length() < limit)
				text += ";";
			break;
		case SDLK_QUOTE:
			if (text.length() < limit)
				text += "'";
			break;
		case SDLK_z:
			if (text.length() < limit)
				text += "z";
			break;
		case SDLK_x:
			if (text.length() < limit)
				text += "x";
			break;
		case SDLK_c:
			if (text.length() < limit)
				text += "c";
			break;
		case SDLK_v:
			if (text.length() < limit && !(SDL_GetModState() & KMOD_CTRL))
				text += "v";
			break;
		case SDLK_b:
			if (text.length() < limit)
				text += "b";
			break;
		case SDLK_n:
			if (text.length() < limit)
				text += "n";
			break;
		case SDLK_m:
			if (text.length() < limit)
				text += "m";
			break;
		case SDLK_COMMA:
			if (text.length() < limit)
				text += ",";
			break;
		case SDLK_PERIOD:
			if (text.length() < limit)
				text += ".";
			break;
		case SDLK_SLASH:
			if (text.length() < limit)
				text += "/";
			break;
		case SDLK_SPACE:
			if (text.length() < limit)
				text += " ";
			break;
		case SDLK_BACKSPACE:
			if (text.length() != 1)
			{
				text.erase(text.end() - 1, text.end());
			}
			break;
		case SDLK_LSHIFT:
			shiftDown = true;
			break;
		case SDLK_RSHIFT:
			shiftDown = true;
			break;
		}
	}
	else if (e->type == SDL_KEYDOWN && shiftDown)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_1:
			if (text.length() < limit)
				text += "!";
			break;
		case SDLK_2:
			if (text.length() < limit)
				text += "@";
			break;
		case SDLK_3:
			if (text.length() < limit)
				text += "#";
			break;
		case SDLK_4:
			if (text.length() < limit)
				text += "$";
			break;
		case SDLK_5:
			if (text.length() < limit)
				text += "%";
			break;
		case SDLK_6:
			if (text.length() < limit)
				text += "^";
			break;
		case SDLK_7:
			if (text.length() < limit)
				text += "&";
			break;
		case SDLK_8:
			if (text.length() < limit)
				text += "*";
			break;
		case SDLK_9:
			if (text.length() < limit)
				text += "(";
			break;
		case SDLK_0:
			if (text.length() < limit)
				text += ")";
			break;
		case SDLK_q:
			if (text.length() < limit)
				text += "Q";
			break;
		case SDLK_w:
			if (text.length() < limit)
				text += "W";
			break;
		case SDLK_e:
			if (text.length() < limit)
				text += "E";
			break;
		case SDLK_r:
			if (text.length() < limit)
				text += "R";
			break;
		case SDLK_t:
			if (text.length() < limit)
				text += "T";
			break;
		case SDLK_y:
			if (text.length() < limit)
				text += "Y";
			break;
		case SDLK_u:
			if (text.length() < limit)
				text += "U";
			break;
		case SDLK_i:
			if (text.length() < limit)
				text += "I";
			break;
		case SDLK_o:
			if (text.length() < limit)
				text += "O";
			break;
		case SDLK_p:
			if (text.length() < limit)
				text += "P";
			break;
		case SDLK_LEFTBRACKET:
			if (text.length() < limit)
				text += "{";
			break;
		case SDLK_RIGHTBRACKET:
			if (text.length() < limit)
				text += "}";
			break;
		case SDLK_BACKSLASH:
			if (text.length() < limit)
				text += "|";
			break;
		case SDLK_BACKQUOTE:
			if (text.length() < limit)
				text += "~";
			break;
		case SDLK_EQUALS:
			if (text.length() < limit)
				text += "+";
			break;
		case SDLK_MINUS:
			if (text.length() < limit)
				text += "_";
			break;
		case SDLK_a:
			if (text.length() < limit)
				text += "A";
			break;
		case SDLK_s:
			if (text.length() < limit)
				text += "S";
			break;
		case SDLK_d:
			if (text.length() < limit)
				text += "D";
			break;
		case SDLK_f:
			if (text.length() < limit)
				text += "F";
			break;
		case SDLK_g:
			if (text.length() < limit)
				text += "G";
			break;
		case SDLK_h:
			if (text.length() < limit)
				text += "H";
			break;
		case SDLK_j:
			if (text.length() < limit)
				text += "J";
			break;
		case SDLK_k:
			if (text.length() < limit)
				text += "K";
			break;
		case SDLK_l:
			if (text.length() < limit)
				text += "L";
			break;
		case SDLK_SEMICOLON:
			if (text.length() < limit)
				text += ":";
			break;
		case SDLK_QUOTE:
			if (text.length() < limit)
				text += "\"";
			break;
		case SDLK_z:
			if (text.length() < limit)
				text += "Z";
			break;
		case SDLK_x:
			if (text.length() < limit)
				text += "X";
			break;
		case SDLK_c:
			if (text.length() < limit)
				text += "C";
			break;
		case SDLK_v:
			if (text.length() < limit && !(SDL_GetModState() & KMOD_CTRL))
				text += "V";
			break;
		case SDLK_b:
			if (text.length() < limit)
				text += "B";
			break;
		case SDLK_n:
			if (text.length() < limit)
				text += "N";
			break;
		case SDLK_m:
			if (text.length() < limit)
				text += "M";
			break;
		case SDLK_COMMA:
			if (text.length() < limit)
				text += "<";
			break;
		case SDLK_PERIOD:
			if (text.length() < limit)
				text += ">";
			break;
		case SDLK_SLASH:
			if (text.length() < limit)
				text += "?";
			break;
		case SDLK_SPACE:
			if (text.length() < limit)
				text += " ";
			break;
		case SDLK_BACKSPACE:
			if (text.length() != 0)
			{
				text.erase(text.end() - 1, text.end());
			}
			break;
		case SDLK_LSHIFT:
			shiftDown = true;
			break;
		case SDLK_RSHIFT:
			shiftDown = true;
			break;
		}
	}
}

string VTextInput::getText()
{
	return text;
}

void VTextInput::setText(string tx)
{
	text = tx;
}