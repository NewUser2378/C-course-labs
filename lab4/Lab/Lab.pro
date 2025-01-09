

# Прописываем зависимости
QT += core gui widgets

# Включаем поддержку C++11
CONFIG += c++11

# Определение для вывода предупреждений о использовании устаревших функций Qt
DEFINES += QT_DEPRECATED_WARNINGS

# Вы можете также заставить код не компилироваться, если он использует устаревшие API
# Расскомментируйте следующую строку, если хотите выключить устаревшие API
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # Отключает все API, устаревшие до Qt 6.0.0

# Здесь указываем исходные файлы
SOURCES += \
    MainWindow.cpp \
    Plot.cpp

HEADERS += \
    MainWindow.h \
    Plot.h

# Правила развертывания по умолчанию
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
