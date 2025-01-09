#ifndef PLOT_H
#define PLOT_H

#include <QSettings>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/q3dscatter.h>
#include <QtDataVisualization/qabstract3dseries.h>
#include <QtGui/QFont>
#include <QtWidgets/QSlider>

using namespace QtDataVisualization;

class Plot : public QObject
{
    Q_OBJECT
  public:
    explicit Plot(Q3DSurface *surface, Q3DScatter *scatter);	//преедаем еще и скейтер
    ~Plot();

    void enableSinSinModel(bool enable);

    void enableSqrtSinModel(bool enable);

    //! [0]
    void toggleModeNone()
    {
        m_graph->setSelectionMode(QAbstract3DGraph::SelectionNone);
        m_sct_graph->setSelectionMode(QAbstract3DGraph::SelectionNone);
    }

    void toggleModeItem()
    {
        m_graph->setSelectionMode(QAbstract3DGraph::SelectionItem);
        m_sct_graph->setSelectionMode(QAbstract3DGraph::SelectionItem);
    }

    //! [0]

    void setBlackToYellowGradient();
    void setGreenToRedGradient();

    void setAxisMinSliderX(QSlider *slider) { m_axisMinSliderX = slider; }
    void setAxisMaxSliderX(QSlider *slider) { m_axisMaxSliderX = slider; }
    void setAxisMinSliderZ(QSlider *slider) { m_axisMinSliderZ = slider; }
    void setAxisMaxSliderZ(QSlider *slider) { m_axisMaxSliderZ = slider; }

    void adjustXMin(int min);
    void adjustXMax(int max);
    void adjustZMin(int min);
    void adjustZMax(int max);

    void setLabelsVisible(bool visible);

    void changeNumberOfStep(int number);

  public Q_SLOTS:
    void changeTheme(int theme);

  private:
    Q3DSurface *m_graph;
    QSurfaceDataProxy *m_sqrtSinProxy;

    QSurfaceDataProxy *m_sinSinProxy;

    QSurface3DSeries *m_sqrtSinSeries;
    QSurface3DSeries *m_sinSinSeries;

    QSlider *m_axisMinSliderX;
    QSlider *m_axisMaxSliderX;
    QSlider *m_axisMinSliderZ;
    QSlider *m_axisMaxSliderZ;

    float m_rangeMinX;
    float m_rangeMinZ;
    float m_stepX;
    float m_stepZ;

    //для скейтера
    Q3DScatter *m_sct_graph;
    QScatterDataProxy *m_sct_sqrtSinProxy;

    QScatterDataProxy *m_sct_sinSinProxy;

    QScatter3DSeries *m_sct_sqrtSinSeries;
    QScatter3DSeries *m_sct_sinSinSeries;

    //

    QValue3DAxis *axisX;
    QValue3DAxis *axisY;
    QValue3DAxis *axisZ;

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);

    void fillSqrtSinProxy();
    void fillSinSinProxy();

    //для скейтера
    void fillSctSqrtSinProxy();
    void fillSctSinSinProxy();
};

#endif
