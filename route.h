#ifndef ROUTE_H
#define ROUTE_H

#include <QString>

class Route
{
public:
    Route();
    QString calculateRoute(const QString &startPoint, const QString &endPoint);  // 路径计算函数
};

#endif // ROUTE_H
