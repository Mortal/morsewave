SOURCES += \
    main.cpp \
    morsegenerator.cpp \
    morsewavesettings.cpp \
    morseconfigurationmodel.cpp \
    morsegenerator.inl \
    morseplayer.cpp

HEADERS += \
    morsewave.h \
    morsegenerator.h \
    morsewavesettings.h \
    morseconfigurationmodel.h \
    morseconfiguration.h \
    morseplayer.h


win32: LIBS += -L$$PWD/libsndfile/lib/ -llibsndfile-1

INCLUDEPATH += $$PWD/libsndfile/include
DEPENDPATH += $$PWD/libsndfile/include

QT += phonon
