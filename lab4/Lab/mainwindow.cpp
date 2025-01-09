#include "mainwindow.h"
#include "plot.h"

#include <QApplication>
#include <QLocale>
#include <QCheckBox>
#include <QSettings>
#include <QStackedWidget>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/qscatterdataproxy.h>
#include <QtGui/QFontDatabase>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

int main(int argc, char **argv)
{
    //! [0]
    QApplication app(argc, argv);	 //сохдаем объект приложения
    Q3DSurface *graph = new Q3DSurface();	 //создаем объект Q3DSurface который будет представлять 3Д-график
                                             //поверхности
    Q3DScatter *scatter = new Q3DScatter();
    QWidget *container = QWidget::createWindowContainer(graph);
    QWidget *scatter_container = QWidget::createWindowContainer(scatter);
    // делаем виджет,используется как контейнер для встраивания объекта в окно QT для отображения в окне QT
    //! [0]
    //!
    //!

    if (!graph->hasContext())
    {
        //Здесь проверяем , удалось ли инициализировать
        //контекст OpenGL для отрисовки графика
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return -1;
    }

    QStackedWidget *graph_container = new QStackedWidget;
    graph_container->addWidget(container);
    graph_container->addWidget(scatter_container);

    graph_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QSize screenSize = graph->screen()->size();	   // получаем размер экрана
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    //устанавливаем минимальный размер контейнера
    container->setMaximumSize(screenSize);	  //устанавливаем максимальный размер контейнера
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //устанавливаем политику размеров контейнеров
    container->setFocusPolicy(Qt::StrongFocus);	   // политика фокусировки

    //! [1]
    QWidget *widget = new QWidget;	  //выделяем памятьи и создаем  основной виджет, который будет содержать остальные
                                      //элементы интерфейса
    QHBoxLayout *hLayout = new QHBoxLayout(widget);	   // горизонтальный макет(расположение элементов по горизонтали)
                                                       // для размещения элементов интерфейса, передаем в
                                                       // качествеаргумента родительский виджет( то есть будем
                                                       // распологаться внутри него)
    QVBoxLayout *vLayout = new QVBoxLayout();	 //аналогично вертикальный макет
    hLayout->addWidget(graph_container);

    // hLayout->addWidget(container, 0);// добавляет в горизонтальный виджет -виджет контейнер (в котором у нас
    // распологается график),
    // также присваивает параметру stretch значение 1, то есть виджет container будет иметь больший вес (больше места)
    // по сравнению с другими виджетами,
    // hLayout->addWidget(scatter_container, 1);
    hLayout->addLayout(vLayout);	// делаем так чтобы внутри горизонтального расположения виджеты располагалсь
                                    // вертикально

    vLayout->setAlignment(Qt::AlignTop);	//устанавливам вертикальное выравнивание элементов внутри vLayout в верхнем
                                            //положении
    //! [1]

    widget->setWindowTitle(QStringLiteral("Lab4"));	   // добавляем название

    QGroupBox *selectionTypeBox = new QGroupBox(QStringLiteral("Plot"));
    QRadioButton *surfaceRadioButton = new QRadioButton(widget);
    surfaceRadioButton->setText(QStringLiteral("Surface"));
    surfaceRadioButton->setChecked(true);

    QRadioButton *scatterRadioButton = new QRadioButton(widget);
    scatterRadioButton->setText(QStringLiteral("Scatter"));
    scatterRadioButton->setChecked(false);

    QVBoxLayout *TypeVBox = new QVBoxLayout;

    selectionTypeBox->setLayout(TypeVBox);

    // Создаем отдельный контейнер для selectionTypeBox
    QVBoxLayout *selectionTypeContainer = new QVBoxLayout;
    selectionTypeContainer->addWidget(selectionTypeBox);

    vLayout->addLayout(selectionTypeContainer);	   // Добавляем контейнер в вертикальный макет vLayout
    // ...
    QGroupBox *modelGroupBox = new QGroupBox(QStringLiteral("Graph Type"));	   // создаем группу элементов с названием
                                                                               // Model

    QRadioButton *sqrtSinModelRB = new QRadioButton(widget);	//кнопка для переключения 1 графика

    sqrtSinModelRB->setText(QStringLiteral("Sinc1"));	 // название для кнопки
    sqrtSinModelRB->setChecked(false);	  // устанавливаем состояние чекбокса (переключателя)

    QRadioButton *sinSinModelRB = new QRadioButton(widget);	   //кнопка для 2 графика
    sinSinModelRB->setText(QStringLiteral("Sinc2"));
    sinSinModelRB->setChecked(false);

    QVBoxLayout *modelVBox = new QVBoxLayout;	 // создаем вертикальный макет(расположение элементов по вертикали) для
                                                 // размещения кнопок в нем по вертиклаи

    modelVBox->addWidget(surfaceRadioButton);
    modelVBox->addWidget(scatterRadioButton);

    TypeVBox->addWidget(sqrtSinModelRB);
    TypeVBox->addWidget(sinSinModelRB);

    modelGroupBox->setLayout(modelVBox);	// размещаем modelVBox при помощи метода setLayout внутри основного
                                            // modelGroupBox

    QGroupBox *selectionGroupBox = new QGroupBox(QStringLiteral("Selection Mode"));	   // создаем группу для выбора
                                                                                       // режима выбора точки на графике

    QRadioButton *modeNoneRB = new QRadioButton(widget);	// создаем кнопку для того, чтобы можно было не выбирать
                                                            // точку на графике
    modeNoneRB->setText(QStringLiteral("No selection"));	// даем ей название
    modeNoneRB->setChecked(false);	  // устанавливаем что это состояние не выбрано (в конце установим, что в начале оно
                                      // будет true)

    QRadioButton *modeItemRB = new QRadioButton(widget);	// аналогично для кнопки, которая может выбирать точку на
                                                            // графике
    modeItemRB->setText(QStringLiteral("Item"));
    modeItemRB->setChecked(false);

    QVBoxLayout *selectionVBox = new QVBoxLayout;	 // создаем группу элементов куда добавляем 2 кнопки Item и No
                                                     // selection
    selectionVBox->addWidget(modeNoneRB);	 // добавляем кнопки в этот контейнер
    selectionVBox->addWidget(modeItemRB);

    selectionGroupBox->setLayout(selectionVBox);	// добавляем контейнер в группу элементов с названием Selection Mode

    QSlider *axisMinSliderX = new QSlider(Qt::Horizontal, widget);	  // создаем ползунок который двигает координаты по
                                                                      // Х со стороны минимума (справа на лево)
    axisMinSliderX->setMinimum(0);	  // устанавливаем минимальное значение по X ( справа) если выставить не 0, то
                                      // координаты по Х начнутся не с -10
    axisMinSliderX->setTickInterval(1);	   // устанавливаем шаг ползунка =1, ТО ЕТСЬ будем останавливаться на
                                           // координатах кратных 1

    axisMinSliderX->setEnabled(true);	 // позволяем пользователю взаимодействовать с ползунком

    QSlider *axisMaxSliderX = new QSlider(Qt::Horizontal, widget);	  // аналогично с ползунком в другую сторону
    axisMaxSliderX->setMinimum(1);
    axisMaxSliderX->setTickInterval(1);
    axisMaxSliderX->setEnabled(true);

    QSlider *axisMinSliderZ = new QSlider(Qt::Horizontal, widget);	  // аналогично по оси Z
    axisMinSliderZ->setMinimum(0);
    axisMinSliderZ->setTickInterval(1);
    axisMinSliderZ->setEnabled(true);
    QSlider *axisMaxSliderZ = new QSlider(Qt::Horizontal, widget);
    axisMaxSliderZ->setMinimum(1);
    axisMaxSliderZ->setTickInterval(1);
    axisMaxSliderZ->setEnabled(true);

    QComboBox *themeList = new QComboBox(widget);	 //добавлеяем список стилей
    themeList->addItem(QStringLiteral("Qt"));
    themeList->addItem(QStringLiteral("Primary Colors"));
    themeList->addItem(QStringLiteral("Digia"));
    themeList->addItem(QStringLiteral("Stone Moss"));
    themeList->addItem(QStringLiteral("Army Blue"));
    themeList->addItem(QStringLiteral("Retro"));
    themeList->addItem(QStringLiteral("Ebony"));
    themeList->addItem(QStringLiteral("Isabelle"));

    QCheckBox *gridCheckBox = new QCheckBox((QStringLiteral("Show grid")));	   //создаем чекбокс для показа сетки

    QGroupBox *colorGroupBox = new QGroupBox(QStringLiteral("Custom gradient"));	//создаем чекбокс для градиентов

    QCheckBox *BorderCheckBox = new QCheckBox(QStringLiteral("Show label border"));	   //создаем чекбокс для границ
                                                                                       //вокруг меток

    QCheckBox *labelCheckBox = new QCheckBox(QStringLiteral("Show label"));	   //создаем чекбокс для показа меток

    QLinearGradient grBtoY(0, 0, 1, 100);
    grBtoY.setColorAt(1.0, Qt::black);
    grBtoY.setColorAt(0.67, Qt::blue);
    grBtoY.setColorAt(0.33, Qt::red);
    grBtoY.setColorAt(0.0, Qt::yellow);
    QPixmap pm(24, 100);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(grBtoY));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(0, 0, 24, 100);
    QPushButton *gradientBtoYPB = new QPushButton(widget);
    gradientBtoYPB->setIcon(QIcon(pm));
    gradientBtoYPB->setIconSize(QSize(24, 100));

    QLinearGradient grGtoR(0, 0, 1, 100);
    grGtoR.setColorAt(1.0, Qt::darkGreen);
    grGtoR.setColorAt(0.5, Qt::yellow);
    grGtoR.setColorAt(0.2, Qt::red);
    grGtoR.setColorAt(0.0, Qt::darkRed);
    pmp.setBrush(QBrush(grGtoR));
    pmp.drawRect(0, 0, 24, 100);
    QPushButton *gradientGtoRPB = new QPushButton(widget);
    gradientGtoRPB->setIcon(QIcon(pm));
    gradientGtoRPB->setIconSize(QSize(24, 100));

    QHBoxLayout *colorHBox = new QHBoxLayout;	 // добавляем горизонтальный макет куда будем добавлять градиенты, чтобы
                                                 // они распологались в строку
    colorHBox->addWidget(gradientBtoYPB);		 // добавляем градиенты
    colorHBox->addWidget(gradientGtoRPB);

    colorGroupBox->setLayout(colorHBox);	//добавляем горизонтальный макет в чекбокс с названием Custom gradient

    QLabel *labelNumberOfStep = new QLabel(QStringLiteral("Label Number of steps"));	//

    QSlider *numberOfStep = new QSlider(Qt::Horizontal, widget);	// создаем горизонтальный ползунок для количества
                                                                    // шагов

    numberOfStep->setMinimum(10);		 // минимальное значение ползунка
    numberOfStep->setMaximum(100);		 // максимум
    numberOfStep->setTickInterval(1);	 // делаем шаг = 1 , то есть при преремещении будем останавливаться на значениях
                                         // кратных 1
    numberOfStep->setEnabled(true);	   // позволяем взаимодействовать пользователю

    vLayout->addWidget(modelGroupBox);	  // добавляем все виджеты в вертикальный макет
    vLayout->addWidget(selectionGroupBox);

    vLayout->addWidget(new QLabel(QStringLiteral("Column range")));	   // добавляем в главный вертикальный макет
                                                                       // названия прежде чем добавим  виджеты
    vLayout->addWidget(axisMinSliderX);
    vLayout->addWidget(axisMaxSliderX);
    vLayout->addWidget(new QLabel(QStringLiteral("Row range")));
    vLayout->addWidget(axisMinSliderZ);
    vLayout->addWidget(axisMaxSliderZ);
    vLayout->addWidget(new QLabel(QStringLiteral("Theme")));
    vLayout->addWidget(themeList);
    vLayout->addWidget(colorGroupBox);

    vLayout->addWidget(gridCheckBox);

    vLayout->addWidget(labelCheckBox);

    vLayout->addWidget(BorderCheckBox);

    vLayout->addWidget(labelNumberOfStep);
    vLayout->addWidget(numberOfStep);

    widget->show();

    Plot *modifier = new Plot(graph, scatter);	  //создаем объект основного класса

    QObject::connect(numberOfStep, &QSlider::valueChanged, modifier, &Plot::changeNumberOfStep);

    QObject::connect(sinSinModelRB, &QRadioButton::toggled, modifier, &Plot::enableSinSinModel);

    QObject::connect(sqrtSinModelRB, &QRadioButton::toggled, modifier, &Plot::enableSqrtSinModel);

    QObject::connect(modeNoneRB, &QRadioButton::toggled, modifier, &Plot::toggleModeNone);

    QObject::connect(modeItemRB, &QRadioButton::toggled, modifier, &Plot::toggleModeItem);

    QObject::connect(axisMinSliderX, &QSlider::valueChanged, modifier, &Plot::adjustXMin);

    QObject::connect(axisMaxSliderX, &QSlider::valueChanged, modifier, &Plot::adjustXMax);

    QObject::connect(axisMinSliderZ, &QSlider::valueChanged, modifier, &Plot::adjustZMin);

    QObject::connect(axisMaxSliderZ, &QSlider::valueChanged, modifier, &Plot::adjustZMax);

    QObject::connect(themeList, SIGNAL(currentIndexChanged(int)), modifier, SLOT(changeTheme(int)));

    QObject::connect(gradientBtoYPB, &QPushButton::pressed, modifier, &Plot::setBlackToYellowGradient);

    QObject::connect(gradientGtoRPB, &QPushButton::pressed, modifier, &Plot::setGreenToRedGradient);

    QObject::connect(gridCheckBox, &QCheckBox::stateChanged, [&](int state) {
        // кнопка отключения сетки в зависимости от состояния в checkBox
        graph->activeTheme()->setGridEnabled(state != Qt::Unchecked);
        scatter->activeTheme()->setGridEnabled(state != Qt::Unchecked);
    });

    QObject::connect(labelCheckBox, &QCheckBox::stateChanged, [&](bool state) {
        modifier->setLabelsVisible(state);	  //отключаем символы

        //подключаем кнопку для границ вокруг значков  в зависимости от состояния checkBox
        BorderCheckBox->setEnabled(state != Qt::Unchecked);
    });

    QObject::connect(BorderCheckBox, &QCheckBox::stateChanged, [&](bool state) {
        graph->activeTheme()->setLabelBorderEnabled(state);
        scatter->activeTheme()->setLabelBorderEnabled(state);
    });

    //
    QObject::connect(scatterRadioButton, &QRadioButton::clicked, [&](bool state) {
        displayScatter(state, graph_container);
    });
    QObject::connect(surfaceRadioButton, &QRadioButton::clicked, [&](bool state) {
        displaySurface(state, graph_container);
    });

    modelVBox->addWidget(surfaceRadioButton);
    modelVBox->addWidget(scatterRadioButton);
    modelGroupBox->setLayout(modelVBox);

    modifier->setAxisMinSliderX(axisMinSliderX);
    modifier->setAxisMaxSliderX(axisMaxSliderX);
    modifier->setAxisMinSliderZ(axisMinSliderZ);
    modifier->setAxisMaxSliderZ(axisMaxSliderZ);

    sqrtSinModelRB->setChecked(true);

    sinSinModelRB->setChecked(false);

    modeItemRB->setChecked(true);
    gridCheckBox->setChecked(true);
    labelCheckBox->setChecked(true);
    BorderCheckBox->setChecked(true);
    themeList->setCurrentIndex(2);

    QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    // Загрузка настроек
    int savedValue = settings.value("sliderValue", 0).toInt();
    numberOfStep->setValue(savedValue);
    sinSinModelRB->setChecked(settings.value("sinSinModelRB", false).toBool());
    sqrtSinModelRB->setChecked(settings.value("sqrtSinModelRB", true).toBool());
    modeItemRB->setChecked(settings.value("modeItemRB", true).toBool());
    modeNoneRB->setChecked(settings.value("modeNoneRB", false).toBool());
    axisMinSliderX->setValue(settings.value("axisMinSliderX", 0).toInt());
    axisMaxSliderX->setValue(settings.value("axisMaxSliderX", 1).toInt());
    axisMinSliderZ->setValue(settings.value("axisMinSliderZ", 0).toInt());
    axisMaxSliderZ->setValue(settings.value("axisMaxSliderZ", 1).toInt());
    themeList->setCurrentIndex(settings.value("themeList", 2).toInt());
    gridCheckBox->setChecked(settings.value("gridCheckBox", true).toBool());
    labelCheckBox->setChecked(settings.value("labelCheckBox", true).toBool());
    BorderCheckBox->setChecked(settings.value("BorderCheckBox", true).toBool());

    // Сохранение настроек перед выходом
    QObject::connect(&app, &QApplication::aboutToQuit, [&]() {
        settings.setValue("sliderValue", numberOfStep->value());
        settings.setValue("sinSinModelRB", sinSinModelRB->isChecked());
        settings.setValue("sqrtSinModelRB", sqrtSinModelRB->isChecked());
        settings.setValue("modeItemRB", modeItemRB->isChecked());
        settings.setValue("modeNoneRB", modeNoneRB->isChecked());
        settings.setValue("axisMinSliderX", axisMinSliderX->value());
        settings.setValue("axisMaxSliderX", axisMaxSliderX->value());
        settings.setValue("axisMinSliderZ", axisMinSliderZ->value());
        settings.setValue("axisMaxSliderZ", axisMaxSliderZ->value());
        settings.setValue("themeList", themeList->currentIndex());
        settings.setValue("gridCheckBox", gridCheckBox->isChecked());
        settings.setValue("labelCheckBox", labelCheckBox->isChecked());
        settings.setValue("BorderCheckBox", BorderCheckBox->isChecked());
        settings.sync();
    });

    return app.exec();
}
