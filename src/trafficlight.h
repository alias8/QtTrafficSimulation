#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <QGraphicsEllipseItem>
#include <QPainter>



class TrafficLight : public QGraphicsEllipseItem
{
public:
    enum class State {Invalid = -1, Off = 0, Manual = 1, Random = 2, Density = 3, Distribution = 4};
    TrafficLight(qreal x, qreal y, QGraphicsItem *parent = 0);
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option,
                 QWidget * widget = 0 );
    void advance(int phase);

    QRadialGradient getGrad() const;
    void setGrad(const QRadialGradient &value);
    double getTraffic_value() const;
    void setTraffic_value(double value);
    State getState() const;
    void setState(State value);
    void setDependants(const QList<TrafficLight *> &value);
    int getId() const { return m_nID; }

private:
    double traffic_value;
    QRadialGradient grad;
    int randomness_value;
    State state;
    bool top;
    QList<TrafficLight *> dependants;
    static int s_nIDGenerator;
    int m_nID;

};





#endif // TRAFFICLIGHT_H
