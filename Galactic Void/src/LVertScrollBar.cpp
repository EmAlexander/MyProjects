#include "LVertScrollBar.h"


LVertScrollBar::LVertScrollBar()
{
}


LVertScrollBar::~LVertScrollBar()
{
}

void LVertScrollBar::handleEvents(SDL_Event *e)
{
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		bool inside = true;

		if (x < colBox.x)
		{
			inside = false;
		}
		else if (x > colBox.x + colBox.w)
		{
			inside = false;
		}
		else if (y < colBox.y)
		{
			inside = false;
		}
		else if (y > colBox.y + colBox.h + bar.getDiment().h)
		{
			inside = false;
		}

		if (!inside)
		{
			over = false;
		}
		switch (e->type)
		{
		case SDL_MOUSEBUTTONDOWN:
			switch (e->button.button)
			{
			case SDL_BUTTON_LEFT:
				if(inside) down = true;
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (e->button.button)
			{
			case SDL_BUTTON_LEFT:
				down = false;
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			if (inside)
			{
				over = true;
				if (down)
				{
					bar.setMenuDiment({ colBox.x + (colBox.w - bar.getDiment().w) / 2, y, bar.getDiment().w, bar.getDiment().h });
					if (bar.getDiment().y > lim2.y)
					{
						bar.setMenuDiment({ colBox.x + (colBox.w - bar.getDiment().w) / 2, lim2.y - bar.getDiment().h + bar.getDiment().h, bar.getDiment().w, bar.getDiment().h });
					}
					if (bar.getDiment().y < lim1.y)
					{
						bar.setMenuDiment({ colBox.x + (colBox.w - bar.getDiment().w) / 2, lim1.y, bar.getDiment().w, bar.getDiment().h });
					}
					float _scale = (double)(colBox.h) / (100);
					float _aDis = bar.getDiment().y - colBox.y;
					float _value;
					if ((_aDis == 0 && _scale == 0) || _scale == 0)
					{
						_value = 0;
					}
					else
					{
						_value = _aDis / _scale;
					}

					value = _value;
				}
				break;
			}
		}
	}
}

void LVertScrollBar::setLim(SDL_Point topLimit, SDL_Point botLimit)
{
	lim1 = topLimit;
	lim2 = botLimit;

	colBox = { lim1.x, lim1.y, lim2.x - lim1.x, lim2.y - lim1.y };

	bar.setMenuDiment({ colBox.x + (colBox.w - bar.getDiment().w) / 2, bar.getDiment().y, bar.getDiment().w, bar.getDiment().h });
}

double LVertScrollBar::getValue()
{
	return value;
}

void LVertScrollBar::render(SDL_Renderer *mren)
{
	bar.render(mren);
}

void LVertScrollBar::free()
{
	bar.free();
}