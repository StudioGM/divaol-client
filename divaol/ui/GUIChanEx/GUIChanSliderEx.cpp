#include "GUIChanSliderEx.h"

namespace gcn
{
	SliderEx::SliderEx()
	{
		setOpaque(false);
		upButton = NULL;
		downButton = NULL;
		markButton = NULL;
		markStart = markEnd = 0;
		markPosition = 0;
		total = 1;

		addMouseListener(this);
		addKeyListener(this);
	}

	SliderEx::~SliderEx()
	{

	}

	void SliderEx::setMarkScale(int start, int end, int total)
	{
		markStart = start;
		markEnd = end;
		this->total = total;
		if (getMarkPosition() > end)
			setMarkPosition(start);
		if (getMarkPosition() < start)
			setMarkPosition(end);
	}

	void SliderEx::setMarkPosition(int v)
	{
		bool p = markPosition != v;
		markPosition = v;
		if (p)
			markerPositionChanged(markPosition);
	}

	int SliderEx::getMarkPosition() const
	{
		return markPosition;
	}

	void SliderEx::adjustMarkerHeight()
	{
		if (total == 0)
		{
			markButton->setSize(markButton->getWidth(), getHeight() - upButton->getHeight() - downButton->getHeight());
		}
		else
		{
			int l = getHeight() - upButton->getHeight() - downButton->getHeight();
			int t = int(l*(double(total - (markEnd - markStart)) / total) + 0.5);
			if (t < markerMinHeight)
				t = markerMinHeight;
			if (t > l)
				t = l;
			markButton->setSize(markButton->getWidth(), t);
		}

		upButton->setPosition(0, 0);
		downButton->setPosition(0, getHeight() - downButton->getHeight());
	}

	void SliderEx::setLook(ButtonEx* up, ButtonEx* down, ButtonEx* mark)
	{
		if (up)
		{
			if (upButton)
			{
				remove(upButton);
				delete upButton;
			}
			upButton = up;
			upButton->addMouseListener(new SliderEx_UpButton_MouseListener());
			add(upButton, 0, 0);
		}
		if (down)
		{
			if (downButton)
			{
				remove(downButton);
				delete downButton;
			}
			downButton = down;
			downButton->addMouseListener(new SliderEx_DownButton_MouseListener());
			add(downButton, 0, getHeight() - downButton->getHeight());
		}
		if (mark)
		{
			if (markButton)
			{
				remove(markButton);
				delete markButton;
			}
			markButton = mark;
			markerMinHeight = markButton->getHeight();
			add(markButton, (getWidth() - markButton->getWidth()) >> 1, 0);

			adjustMarkerHeight();
		}
	}


	void SliderEx::mouseDragged(MouseEvent& mouseEvent)
	{
		adjustMarkerHeight();

		int x = mouseEvent.getX(), y = mouseEvent.getY();

		if (x < 0 || x >= getWidth())
			return;
		
		y -= upButton->getHeight() + markButton->getHeight() / 2;
		int l = getHeight() - upButton->getHeight() - downButton->getHeight() - markButton->getHeight();

		if (y<0)
		{
			setMarkPosition(markStart);
			return;
		}

		if (y >= l)
		{
			setMarkPosition(markEnd);
			return;
		}

		setMarkPosition(markStart + int(y/(double(l) / (markEnd - markStart + 1))));
	}

	void SliderEx::logic()
	{
		adjustMarkerHeight();
		
		if (total == 0)
			markButton->setPosition((getWidth() - markButton->getWidth()) >> 1,
				upButton->getHeight());
		else
		{
			int l = getHeight() - upButton->getHeight() - downButton->getHeight();
			markButton->setPosition((getWidth() - markButton->getWidth()) >> 1,
				upButton->getHeight() + (double(l) / total) * (markPosition - markStart));
		}
	}

	void SliderEx::setMarkerUp()
	{
		int t = markPosition - 1;
		if (t < markStart)
			t = markStart;
		setMarkPosition(t);
	}

	void SliderEx::setMarkerDown()
	{
		int t = markPosition + 1;
		if (t > markEnd)
			t = markEnd;
		setMarkPosition(t);
	}

	void SliderEx::markerPositionChanged(int v)
	{
	}

	void SliderEx::mouseWheelMovedUp(MouseEvent& mouseEvent)
	{
		setMarkerUp();
	}

	void SliderEx::mouseWheelMovedDown(MouseEvent& mouseEvent)
	{
		setMarkerDown();
	}

	//////////////////////////////////////////////////////////////////////////

	void SliderEx_UpButton_MouseListener::mouseClicked(MouseEvent& mouseEvent)
	{
		if (mouseEvent.getButton() == MouseEvent::LEFT)
		{
			((SliderEx*)mouseEvent.getSource()->getParent())->setMarkerUp();
		}
	}

	void SliderEx_DownButton_MouseListener::mouseClicked(MouseEvent& mouseEvent)
	{
		if (mouseEvent.getButton() == MouseEvent::LEFT)
		{
			((SliderEx*)mouseEvent.getSource()->getParent())->setMarkerDown();
		}
	}

}