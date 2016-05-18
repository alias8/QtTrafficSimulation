#include "trafficlight.h"
#include <QDebug>

int TrafficLight::s_nIDGenerator = 0;

TrafficLight::TrafficLight(qreal x, qreal y, QGraphicsItem *parent) :
    QGraphicsEllipseItem(0, 0, 40, 40, parent),
    traffic_value(-1),
    randomness_value(1),
    state(State::Off),
    top(false)
{
    this->setPos(x, y);
    setTransform(QTransform::fromTranslate(-this->boundingRect().width()/2, -this->boundingRect().height()/2));
    grad = QRadialGradient(this->boundingRect().width()/2, this->boundingRect().height()/2, this->boundingRect().width()/2);
    grad.setColorAt(1, Qt::transparent);
    setTraffic_value(0.0);
    m_nID = s_nIDGenerator++;
}

void TrafficLight::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
{
    if(state == State::Invalid || state == State::Off) return;

    this->setBrush(QBrush(grad));
    this->setPen(QPen(QColor(0,0,0,0)));
    painter->setRenderHint(QPainter::Antialiasing, true);
    QGraphicsEllipseItem::paint(painter, option, widget);
}

void TrafficLight::advance(int phase)
{
    if(!phase) return;
    if(top) {
        double average = 0;
        foreach(TrafficLight *temp, dependants) {
            average += temp->getTraffic_value();
        }
        setTraffic_value(average/(dependants.size()*0.8));
    }
    else if(state == State::Random) { // Random
        if(qrand() % 100 != 0) return;
        if(traffic_value == 1.0) randomness_value = 1;
        if(traffic_value == 0.0) randomness_value = 0;

        if(randomness_value == 0) setTraffic_value(getTraffic_value() + 0.02);
        if(randomness_value == 1) setTraffic_value(getTraffic_value() - 0.02);
    }
}

QRadialGradient TrafficLight::getGrad() const
{
    return grad;
}

void TrafficLight::setGrad(const QRadialGradient &value)
{
    grad = value;
}

/*
 * Returns value between 0 and 1
*/
double TrafficLight::getTraffic_value() const
{
    return traffic_value;
}

void TrafficLight::setTraffic_value(double value)
{
    if     (value > 1.0) value = 1.0;
    else if(value < 0.0) value = 0.0;

    if     (value < 0.5) grad.setColorAt(0.5, QColor(510*(value), 255, 0));
    else                 grad.setColorAt(0.5, QColor(255, 255-(510*(value-0.5)), 0));
    traffic_value = value;
    update();
}

TrafficLight::State TrafficLight::getState() const
{
    return state;
}

void TrafficLight::setState(State value)
{
    if(state == State::Invalid) return;
    state = value;
    this->update();
}

void TrafficLight::setDependants(const QList<TrafficLight *> &value)
{
    top = true;
    dependants = value;
}
