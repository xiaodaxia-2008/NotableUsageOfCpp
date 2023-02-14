#include <QObject>
#include <QString>
#include <atomic>
#include <chrono>
#include <format>
#include <iostream>
#include <thread>

class Worker : public QObject
{
    Q_OBJECT
  public:
    ~Worker() { std::cout << "worker is destructed!\n"; }

  public slots:
    void stop() { m_stopped = true; }

    int long_time_compution()
    {
        m_stopped = false;
        std::cout << "start long time compution ...\n";
        int i = 0;
        while (!m_stopped && i++ < 100)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            emit progressValueUpdated(i);
            std::cout << std::format("i: {}\n", i);
        }
        std::cout << std::format("finish long time compution, result: {}\n", i);
        emit resultReady(QString("Result: %1").arg(i));
        return i;
    }

  signals:
    void progressValueUpdated(int);
    void resultReady(QString);

  private:
    std::atomic_bool m_stopped{false};
};