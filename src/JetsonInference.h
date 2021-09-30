#pragma once

#include "System.h"
#include "TensorEngine.h"

#include <QtCore>

class JetsonInference : public QObject
{
    Q_OBJECT
public:
    JetsonInference(QObject *parent = Q_NULLPTR);
    ~JetsonInference();

private:
    tensor::TensorEngine engine_;

private:

public slots:
    void run();
signals:
    void finished();
};
