#include "SelectorEx.h"
#include "GUIChanJsonHelper.h"

namespace gcn
{


	/// -----------------------------------------

	SelectorEx::SelectorEx()
	{
		leftBtn = NULL;
		rightBtn = NULL;
		displayer = NULL;
		setOpaque(false);
	}

	SelectorEx::~SelectorEx()
	{
		if (displayer)
			delete displayer;
	}

	void SelectorEx::setDisplayer(SelectorDisplayer* displayer)
	{
		this->displayer = displayer;
	}

	SelectorDisplayer* SelectorEx::getDisplayer() const
	{
		return displayer;
	}

	SuperButtonEx*  SelectorEx::getLeftBtn() const
	{
		return leftBtn;
	}

	SuperButtonEx*  SelectorEx::getRightBtn() const
	{
		return rightBtn;
	}

	void SelectorEx::adjustButtonPos()
	{
		leftBtn->setPosition(0, getHeight() / 2 -  leftBtn->getHeight() / 2);
		rightBtn->setPosition(getWidth() - rightBtn->getWidth(),getHeight() / 2 -  rightBtn->getHeight() / 2);
	}

	void SelectorEx::draw(Graphics* graphics)
	{
		ContainerEx::draw(graphics);

		if (displayer)
		{
			graphics->pushClipArea(gcn::Rectangle(leftBtn->getWidth(), 0, getWidth() - leftBtn->getWidth() - rightBtn->getWidth(), getHeight()));
			displayer->draw(graphics, getFont(), getAlpha());
			graphics->popClipArea();
		}

		
	}

	SelectorEx* SelectorEx::FromJson(const WJson::Value& conf)
	{
		SelectorEx* s = new SelectorEx();
		s->setDimension(Helper::GetRect(conf[L"desRect"]));
		SuperButtonEx* lb = Helper::CreateButton(conf[L"leftBtn"]);
		SuperButtonEx* rb = Helper::CreateButton(conf[L"rightBtn"]);
		lb->addMouseListener(new Selector_MouseListener());
		rb->addMouseListener(new Selector_MouseListener());
		s->leftBtn = lb;
		s->rightBtn = rb;
		s->add(lb);
		s->add(rb);
		s->adjustButtonPos();
		return s;
	}

	void Selector_MouseListener::mouseClicked(gcn::MouseEvent& mouseEvent)
	{
		SelectorEx *s = dynamic_cast<SelectorEx*>(mouseEvent.getSource()->getParent());
		if (s->getDisplayer())
		{
			if (mouseEvent.getSource() == (Widget*)s->getLeftBtn())
				s->getDisplayer()->ToLast();
			else
				s->getDisplayer()->ToNext();
		}
	}
}