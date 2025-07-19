#pragma once

#include "imagealgorithm.h"
#include "algorithmparams.h"
#include <QMap>
#include <QString>

class AlgorithmFactory
{
public:
    static AlgorithmFactory& instance();

    void registerAlgorithm(ImageAlgorithm* algorithm);
    ImageAlgorithm* createAlgorithm(AlgorithmParams::Algorithm type);

    QStringList algorithmNames() const;
    AlgorithmParams::Algorithm algorithmType(const QString& name) const;

private:
    AlgorithmFactory();
    ~AlgorithmFactory();

    QMap<AlgorithmParams::Algorithm, ImageAlgorithm*> m_algorithms;
    QMap<QString, AlgorithmParams::Algorithm> m_nameToType;
};
