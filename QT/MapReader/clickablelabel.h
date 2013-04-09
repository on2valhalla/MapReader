#ifndef CLICKLABEL_H
#define CLICKLABEL_H
#include <QLabel>
#include "mapreader.h"


class ClickableLabel : public QLabel
{
 
Q_OBJECT
 
public:
    explicit ClickableLabel( const QString& text ="", QWidget * parent=0) :
        QLabel(parent)
    {
        this->setText(text);
    }
    explicit ClickableLabel( QWidget * parent) :
        QLabel(parent)
    {
    }

	~ClickableLabel() {}
	
public slots:
	void clicked( QMouseEvent *event)
    {
         ((MapReader*)this->parentWidget())->clicked(event);
    }
 
protected:
	void mousePressEvent ( QMouseEvent * event )
	{
		emit clicked(event);
    }
};

#endif // CLICKLABEL_H
