#pragma once
#include "generator.hpp"
#include <QObject>
#include <QTimerEvent>
#include <functional>

class CoObj : public QObject
{
    Q_OBJECT
  public:
    CoObj();
    void timerEvent(QTimerEvent *evet) override;

  private:
    tl::generator<int> gen;
    tl::generator<int>::iterator it;
    int timer_id;
};