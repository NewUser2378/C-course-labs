#include "plot.h"

#include <QSettings>
#include <QtCore/qmath.h>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QValue3DAxis>
#include <QtGui/QImage>

using namespace QtDataVisualization;

int sampleCountX = 50;	  //Это количество точек данных (или разбиений) по горизонтальной оси X
int sampleCountZ = 50;	  // аналогично по Z

const float sampleMin = -10.0f;
const float sampleMax = 10.0f;	  //максимальная граница для переменных x и z

Plot::Plot(Q3DSurface *surface, Q3DScatter *scatter) :
    m_graph(surface), m_sct_graph(scatter)	  // используем surface и scatter для инициализации

{
    axisX = new QValue3DAxis;
    axisY = new QValue3DAxis;
    axisZ = new QValue3DAxis;

    m_graph->setAxisX(new QValue3DAxis);	// создаем три оси
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);

    m_sct_graph->setAxisX(new QValue3DAxis);	// создаем три оси в скейтере
    m_sct_graph->setAxisY(new QValue3DAxis);
    m_sct_graph->setAxisZ(new QValue3DAxis);

    //! [0]
    m_sqrtSinProxy = new QSurfaceDataProxy();	 // передаем прокси данные
    m_sqrtSinSeries = new QSurface3DSeries(m_sqrtSinProxy);
    //вторая функция
    m_sinSinProxy = new QSurfaceDataProxy();	// передаем прокси данные
    m_sinSinSeries = new QSurface3DSeries(m_sinSinProxy);
    //! [0]

    //для скейтера

    m_sct_sqrtSinProxy = new QScatterDataProxy();	 // передаем прокси данные
    m_sct_sqrtSinSeries = new QScatter3DSeries(m_sct_sqrtSinProxy);
    m_sct_graph->addSeries(m_sct_sqrtSinSeries);	// Add the series to the graph
    //вторая функция
    m_sct_sinSinProxy = new QScatterDataProxy();	// передаем прокси данные
    m_sct_sinSinSeries = new QScatter3DSeries(m_sct_sinSinProxy);
    m_sct_graph->addSeries(m_sct_sinSinSeries);

    fillSqrtSinProxy();	   // заполняем прокси данные

    fillSinSinProxy();
    //! [0]

    //! [2]
}

Plot::~Plot()
{
    delete m_graph;
    delete m_sct_graph;
}

//! [1]
void Plot::fillSqrtSinProxy()	 //заполняем данные графика функции
{
    float stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);	// считаем шаги для заполнения значений на сетке
    float stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);

    QSurfaceDataArray *dataArray = new QSurfaceDataArray;	 //массив для данных
    dataArray->reserve(sampleCountZ);	 //резервируем место для количества точек данных
    for (int i = 0; i < sampleCountZ; i++)
    {	 // пускаем цикл по вертикальным координатам
        QSurfaceDataRow *newRow = new QSurfaceDataRow(sampleCountX);
        //для каждой из вертикальных координат делаем новую строку размером с количеством точек по Х
        float z = qMin(sampleMax, (i * stepZ + sampleMin));
        int index = 0;
        for (int j = 0; j < sampleCountX; j++)
        {	 // идем по горизонтальным координатам и считаем y
            float x = qMin(sampleMax, (j * stepX + sampleMin));
            float R = qSqrt(z * z + x * x) + 0.01f;	   // добавляем небольшую константу чтобы не делить на 0
            float y = (qSin(R) / R + 0.24f) * 1.61f;	// умножаем для маштабирования
            (*newRow)[index++].setPosition(QVector3D(x, y, z));	   //записываем координаты в текущую позицию newRow
        }
        *dataArray << newRow;	 // после заполнения всех текущих координат добавляем в массив данных
    }

    m_sqrtSinProxy->resetArray(dataArray);

    // для скейтера

    QScatterDataArray *sct_dataArray = new QScatterDataArray;
    sct_dataArray->resize(sampleCountX * sampleCountZ);
    QScatterDataItem *ptrToDataArray = &sct_dataArray->first();

    for (int i = 0; i < sampleCountZ; i++)
    {
        float z = qMin(sampleMax, (i * stepZ + sampleMin));
        for (int j = 0; j < sampleCountX; j++)
        {
            float x = qMin(sampleMax, (j * stepX + sampleMin));
            float R = qSqrt(z * z + x * x) + 0.01f;
            float y = (qSin(R) / R + 0.24f) * 1.61f;

            ptrToDataArray->setPosition(QVector3D(x, y, z));
            ptrToDataArray++;
        }
    }

    m_sct_sqrtSinSeries->dataProxy()->resetArray(sct_dataArray);
}

void Plot::fillSinSinProxy()	//заполняем данные графика функции
{
    float stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
    float stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);

    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(sampleCountZ);
    for (int i = 0; i < sampleCountZ; i++)
    {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(sampleCountX);
        float z = qMin(sampleMax, (i * stepZ + sampleMin));
        int index = 0;
        for (int j = 0; j < sampleCountX; j++)
        {
            float x = qMin(sampleMax, (j * stepX + sampleMin));
            float sincZ = (qAbs(z) < 0.01f) ? 1.0f : (qSin(z) / z);
            float sincX = (qAbs(x) < 0.01f) ? 1.0f : (qSin(x) / x);
            float y = (sincZ * sincX + 0.25f) * 1.5f;
            (*newRow)[index++].setPosition(QVector3D(x, y, z));
        }
        *dataArray << newRow;
    }

    m_sinSinProxy->resetArray(dataArray);

    // для скейтера

    QScatterDataArray *sct_dataArray = new QScatterDataArray;
    sct_dataArray->resize(sampleCountX * sampleCountZ);
    QScatterDataItem *ptrToDataArray = &sct_dataArray->first();
    for (int i = 0; i < sampleCountZ; i++)
    {
        float z = qMin(sampleMax, (i * stepZ + sampleMin));
        for (int j = 0; j < sampleCountX; j++)
        {
            float x = qMin(sampleMax, (j * stepX + sampleMin));
            float sincZ = (qAbs(z) < 0.01f) ? 1.0f : (qSin(z) / z);
            float sincX = (qAbs(x) < 0.01f) ? 1.0f : (qSin(x) / x);
            float y = (sincZ * sincX + 0.25f) * 1.5f;
            ptrToDataArray->setPosition(QVector3D(x, y, z));
            ptrToDataArray++;
        }
    }

    m_sct_sinSinSeries->dataProxy()->resetArray(sct_dataArray);
}

//! [1]

void Plot::enableSqrtSinModel(bool enable)	  // настраиваем отображение графика
{
    if (enable)
    {
        //! [3]
        m_sqrtSinSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        //режим отображения поверхности и данных
        m_sqrtSinSeries->setFlatShadingEnabled(true);
        //указываем использовать плоское затенение для данной серии данных, что может
        //изменить визуальное представление поверхности.
        m_graph->axisX()->setLabelFormat("%.2f");
        m_graph->axisZ()->setLabelFormat("%.2f");	 //устанавливаем формат меток по осям X и Z в формат с двумя
                                                     //десятичными знаками

        m_graph->axisX()->setRange(sampleMin, sampleMax);	 //Устанавливается диапазон для оси X
        m_graph->axisY()->setRange(0.0f, 2.0f);	   //Устанавливается диапазон для оси Y от 0 до 2
        m_graph->axisZ()->setRange(sampleMin, sampleMax);
        m_graph->axisX()->setLabelAutoRotation(30);	   // автоматическая ротация меток осей
        m_graph->axisY()->setLabelAutoRotation(90);
        m_graph->axisZ()->setLabelAutoRotation(30);

        m_graph->removeSeries(m_sinSinSeries);
        m_graph->addSeries(m_sqrtSinSeries);
        //! [3]

        m_rangeMinX = sampleMin;
        m_rangeMinZ = sampleMin;
        m_stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
        m_stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);
        m_axisMinSliderX->setMaximum(sampleCountX - 2);
        m_axisMinSliderX->setValue(0);
        m_axisMaxSliderX->setMaximum(sampleCountX - 1);
        m_axisMaxSliderX->setValue(sampleCountX - 1);
        m_axisMinSliderZ->setMaximum(sampleCountZ - 2);
        m_axisMinSliderZ->setValue(0);
        m_axisMaxSliderZ->setMaximum(sampleCountZ - 1);
        m_axisMaxSliderZ->setValue(sampleCountZ - 1);
        //! [8]
        //для скейтера

        m_sct_sqrtSinSeries->setMesh(QScatter3DSeries::MeshSphere);

        //указываем использовать плоское затенение для данной серии данных, что может
        //изменить визуальное представление поверхности.
        m_sct_graph->axisX()->setLabelFormat("%.2f");
        m_sct_graph->axisZ()->setLabelFormat("%.2f");	 //устанавливаем формат меток по осям X и Z в формат с двумя
                                                         //десятичными знаками

        m_sct_graph->axisX()->setRange(sampleMin, sampleMax);	 //Устанавливается диапазон для оси X
        m_sct_graph->axisY()->setRange(0.0f, 2.0f);	   //Устанавливается диапазон для оси Y от 0 до 2
        m_sct_graph->axisZ()->setRange(sampleMin, sampleMax);
        m_sct_graph->axisX()->setLabelAutoRotation(30);	   // автоматическая ротация меток осей
        m_sct_graph->axisY()->setLabelAutoRotation(90);
        m_sct_graph->axisZ()->setLabelAutoRotation(30);

        m_sct_graph->removeSeries(m_sct_sinSinSeries);
        m_sct_graph->addSeries(m_sct_sqrtSinSeries);
    }
}

void Plot::enableSinSinModel(bool enable)
{
    if (enable)
    {

        m_sinSinSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        m_sinSinSeries->setFlatShadingEnabled(true);

        axisX->setLabelFormat("%.2f");
        axisZ->setLabelFormat("%.2f");
        axisX->setRange(sampleMin, sampleMax);
        axisY->setRange(0.0f, 2.0f);
        axisZ->setRange(sampleMin, sampleMax);
        axisX->setLabelAutoRotation(30);
        axisY->setLabelAutoRotation(90);
        axisZ->setLabelAutoRotation(30);

        m_graph->removeSeries(m_sqrtSinSeries);
        m_graph->addSeries(m_sinSinSeries);

        m_rangeMinX = sampleMin;
        m_rangeMinZ = sampleMin;
        m_stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
        m_stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);
        m_axisMinSliderX->setMaximum(sampleCountX - 2);
        m_axisMinSliderX->setValue(0);
        m_axisMaxSliderX->setMaximum(sampleCountX - 1);
        m_axisMaxSliderX->setValue(sampleCountX - 1);
        m_axisMinSliderZ->setMaximum(sampleCountZ - 2);
        m_axisMinSliderZ->setValue(0);
        m_axisMaxSliderZ->setMaximum(sampleCountZ - 1);
        m_axisMaxSliderZ->setValue(sampleCountZ - 1);

        //для скейтера
        m_sct_sinSinSeries->setMesh(QScatter3DSeries::MeshSphere);
        m_sct_graph->axisX()->setLabelFormat("%.2f");
        m_sct_graph->axisZ()->setLabelFormat("%.2f");	 //устанавливаем формат меток по осям X и Z в формат с двумя
                                                         //десятичными знаками

        m_sct_graph->axisX()->setRange(sampleMin, sampleMax);	 //Устанавливается диапазон для оси X
        m_sct_graph->axisY()->setRange(0.0f, 2.0f);	   //Устанавливается диапазон для оси Y от 0 до 2
        m_sct_graph->axisZ()->setRange(sampleMin, sampleMax);
        m_sct_graph->axisX()->setLabelAutoRotation(30);	   // автоматическая ротация меток осей
        m_sct_graph->axisY()->setLabelAutoRotation(90);
        m_sct_graph->axisZ()->setLabelAutoRotation(30);

        m_sct_graph->removeSeries(m_sct_sqrtSinSeries);
        m_sct_graph->addSeries(m_sct_sinSinSeries);
    }
}

void Plot::adjustXMin(int min)
{	 //настраиваем минимальные значения диапазона по оси Х на значение min которое передаем
    float minX = m_stepX * float(min) + m_rangeMinX;
    //вычисляем новое минимальное значение по оси X:
    // m_rangeMinX - минимальное значение по этой оси в данном диапазоне
    // умножение min на шаг m_stepX преобразует в значение с учетом координат графика
    int max = m_axisMaxSliderX->value();	//извлекаем текущее максимальное значение по оси X из слайдера
                                            //m_axisMaxSliderX
    if (min >= max)
    {	 // такого не должно быть
        max = min + 1;
        m_axisMaxSliderX->setValue(max);	// устанавливаем новое значение  в слайдере m_axisMaxSliderX,
        //чтобы обновить его визуальное представление
    }
    float maxX = m_stepX * max + m_rangeMinX;	 // аналогично считаем новое максимальное значение

    setAxisXRange(minX, maxX);	  // устанавливаем диапазон по оси Х
}

void Plot::adjustXMax(int max)
{
    float maxX = m_stepX * float(max) + m_rangeMinX;

    int min = m_axisMinSliderX->value();
    if (max <= min)
    {
        min = max - 1;
        m_axisMinSliderX->setValue(min);
    }
    float minX = m_stepX * min + m_rangeMinX;

    setAxisXRange(minX, maxX);
}

void Plot::adjustZMin(int min)
{
    float minZ = m_stepZ * float(min) + m_rangeMinZ;

    int max = m_axisMaxSliderZ->value();
    if (min >= max)
    {
        max = min + 1;
        m_axisMaxSliderZ->setValue(max);
    }
    float maxZ = m_stepZ * max + m_rangeMinZ;

    setAxisZRange(minZ, maxZ);
}

void Plot::adjustZMax(int max)
{
    float maxX = m_stepZ * float(max) + m_rangeMinZ;

    int min = m_axisMinSliderZ->value();
    if (max <= min)
    {
        min = max - 1;
        m_axisMinSliderZ->setValue(min);
    }
    float minX = m_stepZ * min + m_rangeMinZ;

    setAxisZRange(minX, maxX);
}

//! [5]
void Plot::setAxisXRange(float min, float max)
{
    m_graph->axisX()->setRange(min, max);	 //обращаемся к объекту поверхности графика
    // и обращаемся к оси Х , затем вызываем метод,чтобы утсановить новый диапозон для этой оси
    m_sct_graph->axisX()->setRange(min, max);
}

void Plot::setAxisZRange(float min, float max)
{
    m_graph->axisZ()->setRange(min, max);
    m_sct_graph->axisZ()->setRange(min, max);
}
//! [5]

//! [6]
void Plot::changeTheme(int theme)
{
    m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
    m_sct_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
    //сначала получаем текущий стиль ,затем вызываем setType  для установки новой темы
    //созданной с помощью конструктора Q3DTheme::Theme(theme).
}
//! [6]

void Plot::setBlackToYellowGradient()
{
    //! [7]
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    //для скейтера
    m_sct_graph->seriesList().at(0)->setBaseGradient(gr);
    m_sct_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    //! [7]
}

void Plot::setGreenToRedGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    //для скейтера
    m_sct_graph->seriesList().at(0)->setBaseGradient(gr);
    m_sct_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void Plot::changeNumberOfStep(int number)	 // меняем количество шагов
{
    sampleCountX = number;
    sampleCountZ = number;
    fillSinSinProxy();	  // пересчитываем данные графиков
    fillSqrtSinProxy();
}

void Plot::setLabelsVisible(bool visible)
{
    if (visible)
    {
        // включаем значки
        m_graph->axisX()->setLabelFormat("%.2f");
        m_graph->axisY()->setLabelFormat("%.2f");
        m_graph->axisZ()->setLabelFormat("%.2f");

        m_sct_graph->axisX()->setLabelFormat("%.2f");
        m_sct_graph->axisY()->setLabelFormat("%.2f");
        m_sct_graph->axisZ()->setLabelFormat("%.2f");
    }
    else
    {
        // убираем значки
        m_sct_graph->axisX()->setLabelFormat("");
        m_sct_graph->axisY()->setLabelFormat("");
        m_sct_graph->axisZ()->setLabelFormat("");

        m_graph->axisX()->setLabelFormat("");
        m_graph->axisY()->setLabelFormat("");
        m_graph->axisZ()->setLabelFormat("");
    }
}

